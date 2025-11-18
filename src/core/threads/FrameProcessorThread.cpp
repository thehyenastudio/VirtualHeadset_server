#include "core/threads/FrameProcessorThread.h"

FrameProcessorThread::FrameProcessorThread(Queue<FramePtr>* queue, Queue<PacketPtr>* outQueue) : wxThread(wxTHREAD_JOINABLE)
{
	m_queue = queue;
	m_outQueue = outQueue;

	m_encoder.Init();
}

wxThread::ExitCode FrameProcessorThread::Entry()
{
	wxLog::SetThreadActiveTarget(wxLog::GetActiveTarget());
	SetThreadDescription(GetCurrentThread(), L"ProcessorThread");

	EyeParams leftParams{
		960, 1080,   // target width/height
		960, 1080,   // crop width/height
		0, 0,        // crop X/Y
		0.3, 0.05 // lens distortion coefficients
	};

	EyeParams rightParams{
		960, 1080,
		960, 1080,
		0, 0,
		0.3, 0.05
	};

	AVRational tb{ 1, 60 };

	leftEyeFilter = new EyeFilter(1920, 1080, AV_PIX_FMT_BGRA, tb, leftParams);
	rightEyeFilter = new EyeFilter(1920, 1080, AV_PIX_FMT_BGRA, tb, rightParams);

	leftEyeFilter->Init();
	rightEyeFilter->Init();

	sbsFilter = new SBSFilter(1920, 1080, 5, AV_PIX_FMT_BGRA, tb);
	sbsFilter->Init();

	while (!TestDestroy())
	{
		FramePtr frame;
		if (m_queue->WaitAndPop(frame, this))
		{
			if (frame)
			{
				auto leftFuture = std::async(std::launch::async, [&] {
					return leftEyeFilter->Filter(frame.get());
				});

				auto rightFuture = std::async(std::launch::async, [&] {
					return rightEyeFilter->Filter(frame.get());
				});

				// Wait for both results
				AVFrame* leftEye = leftFuture.get();
				AVFrame* rightEye = rightFuture.get();

				AVFrame* filtered = sbsFilter->Filter(leftEye, rightEye);
				FramePtr filteredP = FramePtr(av_frame_clone(filtered));
				// Setting preview
				if (WindowManager::Instance().PreviewWindowShown()) // Make safe to check
				{
					PreviewWindow* preview = WindowManager::Instance().GetPreviewWindow();
					wxCommandEvent evt(EVT_NEW_FRAME);
					evt.SetClientData(av_frame_clone(filtered));
					wxQueueEvent(preview, evt.Clone());
				}

				m_outQueue->Push(std::move(m_encoder.EncodePacket(std::move(filteredP))));
			}
		}
		else
		{
			break;
		}
	}
	return (wxThread::ExitCode) 0;
}
