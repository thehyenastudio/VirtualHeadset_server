#include "core/frame_capture/DXGICapture.h"

bool DXGICapture::Init()
{
	static const D3D_DRIVER_TYPE driverTypes[] = {
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	static const D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_1
	};

	bool deviceCreated = false;
	for (auto dt : driverTypes)
	{
		HRESULT hr = D3D11CreateDevice(
			nullptr,
			dt,
			nullptr,
			0,
			featureLevels,
			static_cast<UINT>(std::size(featureLevels)),
			D3D11_SDK_VERSION,
			&m_device,
			&m_featureLevel,
			&m_deviceContext
		);
		if (SUCCEEDED(hr))
		{
			deviceCreated = true;
			break;
		}
		m_device.Release();
		m_deviceContext.Release();
	}
	if (!deviceCreated)
	{
		return false;
	}

	CComPtr<IDXGIDevice> dxgiDevice;
	HRESULT hr = m_device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
	if (FAILED(hr) || !dxgiDevice)
	{
		return false;
	}

	CComPtr<IDXGIAdapter> adapter;
	hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&adapter));
	if (FAILED(hr) || !adapter)
	{
		return false;
	}

	CComPtr<IDXGIOutput> output;
	// For simplicity choose first output (primary monitor). A fuller impl would select by index or name.
	hr = adapter->EnumOutputs(0, &output);
	if (FAILED(hr) || !output)
	{
		return false;
	}

	CComPtr<IDXGIOutput1> output1;
	hr = output->QueryInterface(__uuidof(IDXGIOutput1), reinterpret_cast<void**>(&output1));
	if (FAILED(hr) || !output1)
	{
		return false;
	}

	hr = output1->DuplicateOutput(m_device, &m_outputDuplication);
	if (FAILED(hr) || !m_outputDuplication)
	{
		return false;
	}

	return true;
}

AVFrame* DXGICapture::CaptureFrame()
{
	if (!m_outputDuplication)
	{
		return nullptr;
	}

	// Acquire the next frame (with small retry loop for the "no new frame" case)
	CComPtr<IDXGIResource> desktopResource;
	DXGI_OUTDUPL_FRAME_INFO frameInfo{};
	HRESULT hr = E_FAIL;
	const int timeoutMs = 500;
	for (int attempt = 0; attempt < 10; ++attempt)
	{
		hr = m_outputDuplication->AcquireNextFrame(timeoutMs, &frameInfo, &desktopResource);
		if (SUCCEEDED(hr) && frameInfo.LastPresentTime.QuadPart == 0)
		{
			desktopResource.Release();
			m_outputDuplication->ReleaseFrame();
			Sleep(1);
			continue;
		}
		break;
	}
	if (FAILED(hr))
	{
		return nullptr;
	}

	// =================================================== 
	// FIXME: HOT FIX
	if (!desktopResource)
	{
		// No resource was provided — release the frame and retry or fail safely.
		m_outputDuplication->ReleaseFrame();
		return nullptr; // or continue the retry loop depending on desired behavior
	}
	// =================================================

	// Query texture
	CComPtr<ID3D11Texture2D> texDesktop;
	hr = desktopResource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&texDesktop)); // TODO: error here FIX with CComPtr
	if (FAILED(hr) || !texDesktop)
	{
		// release frame if we didn't already
		m_outputDuplication->ReleaseFrame();
		return nullptr;
	}

	// Ensure format is a BGRA UNORM variant we can read directly
	DXGI_OUTDUPL_DESC duplDesc{};
	m_outputDuplication->GetDesc(&duplDesc);
	if (!isSupportedFormat(duplDesc.ModeDesc.Format))
	{
		m_outputDuplication->ReleaseFrame();
		return nullptr;
	}

	// Create a staging texture sized to the output and copy the acquired texture into it
	D3D11_TEXTURE2D_DESC desc{};
	desc.Width = duplDesc.ModeDesc.Width;
	desc.Height = duplDesc.ModeDesc.Height;
	desc.Format = duplDesc.ModeDesc.Format;
	desc.ArraySize = 1;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_STAGING;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	desc.BindFlags = 0;
	desc.MiscFlags = 0;

	CComPtr<ID3D11Texture2D> staging;
	hr = m_device->CreateTexture2D(&desc, nullptr, &staging);
	if (FAILED(hr) || !staging)
	{
		m_outputDuplication->ReleaseFrame();
		return nullptr;
	}

	m_deviceContext->CopyResource(staging, texDesktop);

	// Map and copy to CPU memory
	D3D11_MAPPED_SUBRESOURCE mapped{};
	hr = m_deviceContext->Map(staging, 0, D3D11_MAP_READ, 0, &mapped);
	if (FAILED(hr))
	{
		m_outputDuplication->ReleaseFrame();
		return nullptr;
	}

	AVFrame* frame = av_frame_alloc();
	if (!frame)
	{
		m_deviceContext->Unmap(staging, 0);
		m_outputDuplication->ReleaseFrame();
		return nullptr;
	}

	frame->format = AV_PIX_FMT_BGRA;
	frame->width = static_cast<int>(desc.Width);
	frame->height = static_cast<int>(desc.Height);

	if (av_frame_get_buffer(frame, 32) < 0)
	{
		av_frame_free(&frame);
		m_deviceContext->Unmap(staging, 0);
		m_outputDuplication->ReleaseFrame();
		return nullptr;
	}

	for (int y = 0; y < frame->height; ++y)
	{
		std::memcpy(
			frame->data[0] + y * frame->linesize[0],
			reinterpret_cast<uint8_t*>(mapped.pData) + y * mapped.RowPitch,
			frame->width * 4
		);
	}

	m_deviceContext->Unmap(staging, 0);
	m_outputDuplication->ReleaseFrame();

	return frame;
}

bool DXGICapture::isSupportedFormat(DXGI_FORMAT fmt)
{
	return fmt == DXGI_FORMAT_B8G8R8A8_UNORM
		|| fmt == DXGI_FORMAT_B8G8R8X8_UNORM
		|| fmt == DXGI_FORMAT_B8G8R8A8_TYPELESS
		|| fmt == DXGI_FORMAT_B8G8R8A8_UNORM_SRGB
		|| fmt == DXGI_FORMAT_B8G8R8X8_TYPELESS
		|| fmt == DXGI_FORMAT_B8G8R8X8_UNORM_SRGB;
}
