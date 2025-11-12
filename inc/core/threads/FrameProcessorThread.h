#pragma once
#include <wx/log.h>
#include <wx/thread.h>

#include <windows.h>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
}

#include "core/Queue.h"
#include "core/WindowManager.h"
#include "core/filters/SBSFilter.h"
#include "core/filters/EyeFilter.h"
#include "utils/FFmpegUtils.h"

class FrameProcessorThread : public wxThread
{
public:
	FrameProcessorThread(Queue<FramePtr>* queue, Queue<PacketPtr>* outQueue);
	virtual ExitCode Entry() override;

private:
	Queue<FramePtr>* m_queue = nullptr;
	Queue<PacketPtr>* m_outQueue = nullptr;

	SBSFilter* sbsFilter = nullptr;
	EyeFilter* leftEyeFilter = nullptr;
	EyeFilter* rightEyeFilter = nullptr;

	PacketPtr EncodePacket(FramePtr frame);
};

