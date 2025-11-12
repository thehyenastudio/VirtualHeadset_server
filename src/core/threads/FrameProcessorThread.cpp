#include "core/threads/FrameProcessorThread.h"

FrameProcessorThread::FrameProcessorThread(Queue<FramePtr>* queue, Queue<PacketPtr>* outQueue) : wxThread(wxTHREAD_JOINABLE)
{
	m_queue = queue;
	m_outQueue = outQueue;
}

wxThread::ExitCode FrameProcessorThread::Entry()
{
	wxLog::SetThreadActiveTarget(wxLog::GetActiveTarget());

	SetThreadDescription(GetCurrentThread(), L"ProcessorThread");

	while (!TestDestroy())
	{
		FramePtr frame;
		if (m_queue->WaitAndPop(frame, this))
		{
			if (frame)
			{
				// Setting preview
				if (WindowManager::Instance().PreviewWindowShown())
				{
					PreviewWindow* preview = WindowManager::Instance().GetPreviewWindow();
					wxCommandEvent evt(EVT_NEW_FRAME);
					evt.SetClientData(av_frame_clone(frame.get()));
					wxQueueEvent(preview, evt.Clone());
				}

				wxLogMessage("Processing frame %dX%d", frame.get()->width, frame.get()->height);

				m_outQueue->Push(std::move(EncodePacket(std::move(frame))));
			}
		}
		else
		{
			break;
		}
	}
	return (wxThread::ExitCode) 0;
}

PacketPtr FrameProcessorThread::EncodePacket(FramePtr frame)
{
	return PacketPtr(nullptr);
}
