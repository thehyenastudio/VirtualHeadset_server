#include "core/threads/CaptureProcessorThread.h"

CaptureProcessorThread::CaptureProcessorThread(Queue<FramePtr>* queue) : wxThread(wxTHREAD_JOINABLE)
{
	m_queue = queue;
}

wxThread::ExitCode CaptureProcessorThread::Entry()
{
	wxLog::SetThreadActiveTarget(wxLog::GetActiveTarget());
	SetThreadDescription(GetCurrentThread(), L"CaptureThread");

	while (!TestDestroy())
	{
		AVFrame* raw = m_capturer.GetFrame();
		if (raw)
		{
			FramePtr frame(raw);
			m_queue->Push(std::move(frame));
		}
		wxThread::Sleep(16); // TODO: DEBUG NECCESSARY
	}

	return (wxThread::ExitCode) 0;
}
