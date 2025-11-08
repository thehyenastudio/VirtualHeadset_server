#pragma once
#include <wx/log.h>
#include "DXGICapture.h"
#include "GDICapture.h"
#include "FrameBGRA.h"

enum class CaptureType
{
	SCREEN_CAPTURE, // NOW SUPORTED ONLY THIS TYPE
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
	CaptureDevice();
	~CaptureDevice();

	bool Init(CaptureMethod method, CaptureType type = CaptureType::SCREEN_CAPTURE);
	FrameBGRA GetFrame();
private:
	CaptureType type;
	CaptureBase* capturer = nullptr;
};

