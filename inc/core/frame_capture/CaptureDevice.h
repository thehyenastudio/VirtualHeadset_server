#pragma once
#include <wx/log.h>

extern "C" {
#include <libavutil/frame.h>
}

#include "DXGICapture.h"
#include "GDICapture.h"

enum class CaptureType
{
	SCREEN_CAPTURE, // TODO: NOW SUPORTED ONLY THIS TYPE
	WINDOW_CAPTURE,
	GAME_CAPTURE
};

enum class CaptureMethod
{
	DXGI,
	GDI
};

class CaptureDevice
{
public:
	CaptureDevice(CaptureMethod method, CaptureType type = CaptureType::SCREEN_CAPTURE);
	~CaptureDevice();

	bool Init(CaptureMethod method, CaptureType type = CaptureType::SCREEN_CAPTURE);
	AVFrame* GetFrame();
private:
	CaptureType type;
	CaptureBase* capturer = nullptr;
};

