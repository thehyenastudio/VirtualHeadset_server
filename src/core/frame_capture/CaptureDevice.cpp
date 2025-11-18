#include "core/frame_capture/CaptureDevice.h"

CaptureDevice::CaptureDevice(CaptureMethod method, CaptureType type)
{
	this->Init(method, type);
}

CaptureDevice::~CaptureDevice()
{
	delete capturer;
	capturer = nullptr;
}

bool CaptureDevice::Init(CaptureMethod method, CaptureType type)
{
	if (type != CaptureType::SCREEN_CAPTURE)
	{
		wxLogError("Only SCREEN_CAPTURE is supported currently.");
		return false;
	}

	switch (method)
	{
		case CaptureMethod::DXGI:
			capturer = new DXGICapture();
			break;
		case CaptureMethod::GDI:
			capturer = new GDICapture();
			break;
	}

	if (!capturer->Init())
	{
		wxLogError("Failed to initialize CaptureFrame device.");
		delete capturer;
		capturer = nullptr;
		return false;
	}

	return true;
}

AVFrame* CaptureDevice::GetFrame()
{
	return capturer->CaptureFrame();
}
