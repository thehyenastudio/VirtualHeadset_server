#pragma once
#include <wx/thread.h>

#include <windows.h>

#include "core/frame_capture/CaptureDevice.h"
#include "core/Queue.h"
#include "utils/FFmpegUtils.h"

class CaptureProcessorThread : public wxThread
{
public:
	CaptureProcessorThread(Queue<FramePtr>* queue);

protected:
	virtual ExitCode Entry() override;

private:
	Queue<FramePtr>* m_queue = nullptr;

	CaptureDevice m_capturer = {
		CaptureMethod::DXGI,
		CaptureType::SCREEN_CAPTURE
	};
};

