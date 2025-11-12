#pragma once
#include <wx/log.h>

extern "C" {
#include <libavutil/frame.h>
#include <libavutil/imgutils.h>
}

class CaptureBase
{
public:
	virtual ~CaptureBase() = default;

	virtual bool Init() = 0;
	virtual AVFrame* CaptureFrame() = 0;
};

