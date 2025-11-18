#pragma once
#include <wx/log.h>
#include <wx/thread.h>

#include <future>
#include <windows.h>

#include "core/filters/EyeFilter.h"
#include "core/filters/SBSFilter.h"
#include "core/FrameEncoder.h"
#include "core/Queue.h"
#include "core/WindowManager.h"
#include "utils/FFmpegUtils.h"

class FrameProcessorThread : public wxThread
{
public:
	FrameProcessorThread(Queue<FramePtr>* queue, Queue<PacketPtr>* outQueue);
	virtual ExitCode Entry() override;

	AVCodecContext* GetEncoderContext() const { return m_encoder.GetContext(); }

private:
	FrameEncoder m_encoder;

	Queue<FramePtr>* m_queue = nullptr;
	Queue<PacketPtr>* m_outQueue = nullptr;

	SBSFilter* sbsFilter = nullptr;
	EyeFilter* leftEyeFilter = nullptr;
	EyeFilter* rightEyeFilter = nullptr;
};

