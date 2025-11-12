#pragma once
#include <wx/app.h>

#include <fstream>

#include "config.h"
#include "core/threads/CaptureProcessorThread.h"
#include "core/threads/FrameProcessorThread.h"
#include "core/threads/NetworkThread.h"
#include "core/WindowManager.h"
#include "Tray.h"

class Application : public wxApp
{
public:
	virtual bool OnInit() override;
	virtual int OnExit() override;

	void Start();
	void Stop();

private:
	std::ofstream* m_logStream = nullptr;

	Tray* m_tray = nullptr;

	Queue<FramePtr> m_frameQueue;
	Queue<PacketPtr> m_packetQueue;

	CaptureProcessorThread* m_captureProcessorThread = nullptr;
	FrameProcessorThread* m_frameProcessorThread = nullptr;
	NetworkThread* m_networkThread = nullptr;
};