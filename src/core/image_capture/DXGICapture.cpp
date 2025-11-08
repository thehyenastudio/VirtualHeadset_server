#include <core/image_capture/DXGICapture.h>

bool DXGICapture::Init() {
    if (!createDevice()) return false;
    if (!createOutputDuplication()) return false;
    return true;
}

FrameBGRA DXGICapture::CaptureFrame()
{
    if (!m_outputDuplication) {
        return {};
    }

    // Acquire the next frame (with small retry loop for the "no new frame" case)
    CComPtr<IDXGIResource> desktopResource;
    DXGI_OUTDUPL_FRAME_INFO frameInfo{};
    HRESULT hr = E_FAIL;
    const int timeoutMs = 500;
    for (int attempt = 0; attempt < 10; ++attempt) {
        hr = m_outputDuplication->AcquireNextFrame(timeoutMs, &frameInfo, &desktopResource);
        if (SUCCEEDED(hr) && frameInfo.LastPresentTime.QuadPart == 0) {
            // got S_OK but not a real frame yet (as in reference samples)
            desktopResource.Release();
            m_outputDuplication->ReleaseFrame();
            Sleep(1);
            continue;
        }
        break;
    }
    if (FAILED(hr)) {
        return {};
    }

    // Query texture
    CComPtr<ID3D11Texture2D> texDesktop;
    hr = desktopResource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&texDesktop));
    if (FAILED(hr) || !texDesktop) {
        // release frame if we didn't already
        m_outputDuplication->ReleaseFrame();
        return {};
    }

    // Ensure format is a BGRA UNORM variant we can read directly
    DXGI_OUTDUPL_DESC duplDesc{};
    m_outputDuplication->GetDesc(&duplDesc);
    if (!isSupportedFormat(duplDesc.ModeDesc.Format)) {
        m_outputDuplication->ReleaseFrame();
        return {};
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
    if (FAILED(hr) || !staging) {
        m_outputDuplication->ReleaseFrame();
        return {};
    }

    m_deviceContext->CopyResource(staging, texDesktop);

    // Map and copy to CPU memory
    D3D11_MAPPED_SUBRESOURCE mapped{};
    hr = m_deviceContext->Map(staging, 0, D3D11_MAP_READ, 0, &mapped);
    if (FAILED(hr)) {
        m_outputDuplication->ReleaseFrame();
        return {};
    }

    FrameBGRA image;
    image.width = static_cast<int>(desc.Width);
    image.height = static_cast<int>(desc.Height);
    image.rowPitch = static_cast<int>(mapped.RowPitch);
    image.bytes.resize(static_cast<size_t>(image.rowPitch) * image.height);
    std::memcpy(image.bytes.data(), mapped.pData, image.bytes.size());
    m_deviceContext->Unmap(staging, 0);

    // Release the frame after we've copied data out
    m_outputDuplication->ReleaseFrame();

    return image;
}

bool DXGICapture::createDevice()
{
    if (m_device) return true;

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

    for (auto dt : driverTypes) {
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
        if (SUCCEEDED(hr)) return true;
        m_device.Release();
        m_deviceContext.Release();
    }
    return false;
}

bool DXGICapture::createOutputDuplication()
{
    if (!m_device) return false;

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

bool DXGICapture::isSupportedFormat(DXGI_FORMAT fmt)
{
    return fmt == DXGI_FORMAT_B8G8R8A8_UNORM
        || fmt == DXGI_FORMAT_B8G8R8X8_UNORM
        || fmt == DXGI_FORMAT_B8G8R8A8_TYPELESS
        || fmt == DXGI_FORMAT_B8G8R8A8_UNORM_SRGB
        || fmt == DXGI_FORMAT_B8G8R8X8_TYPELESS
        || fmt == DXGI_FORMAT_B8G8R8X8_UNORM_SRGB;
}
