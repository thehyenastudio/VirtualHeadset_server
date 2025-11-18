#include "Application.h"

bool Application::OnInit()
{
	if (!wxApp::OnInit())
	{
		return false;
	}

	if (Tray::IsAvailable())
	{
		this->SetAppName(Config::APP_NAME);
		this->SetVendorDisplayName(Config::APP_VENDOR);

		wxFileName logFile(wxStandardPaths::Get().GetExecutablePath());
		logFile.SetFullName("info.log");
		wxString path = logFile.GetFullPath();

		m_logStream = new std::ofstream(path.ToStdString(), std::ios::out | std::ios::app);

		wxLog::SetActiveTarget(new wxLogStream(m_logStream));
		wxLog::SetLogLevel(wxLOG_Debug);
		wxLog::SetTimestamp("%m/%d/%Y %I:%M:%S %p");
		wxLogMessage(_("Virtual Headset server starting..."));

		Config::Instance().Load();

		m_tray = new Tray();

		if (!Config::Instance().GetStartMinimized())
		{
			WindowManager::Instance().GetMainWindow()->Show();
		}

		wxLogMessage(_("Virtual Headset server started"));

		this->Start();

		return true;
	}
	else
	{
		return false;
	}
}

int Application::OnExit()
{
	this->Stop();

	wxLog::SetActiveTarget(nullptr);
	if (m_logStream)
	{
		m_logStream->close();
		delete m_logStream;
		m_logStream = nullptr;
	}
	return 0;
}

void Application::Start()
{
	m_captureProcessorThread = new CaptureProcessorThread(&m_frameQueue);
	if (m_captureProcessorThread->Create() != wxTHREAD_NO_ERROR)
	{
		wxLogError("Failed to create capture thread");
	}
	else
	{
		m_captureProcessorThread->Run();
	}

	m_frameProcessorThread = new FrameProcessorThread(&m_frameQueue, &m_packetQueue);
	if (m_frameProcessorThread->Create() != wxTHREAD_NO_ERROR)
	{
		wxLogError("Failed to create processor thread");
	}
	else
	{
		m_frameProcessorThread->Run();
	}

	m_networkThread = new NetworkThread(&m_packetQueue, "192.168.0.104", 5000);
	m_networkThread->InitMuxer(m_frameProcessorThread->GetEncoderContext());

	if (m_networkThread->Create() != wxTHREAD_NO_ERROR)
	{
		wxLogError("Failed to create processor thread");
	}
	else
	{
		m_networkThread->Run();
	}
}

void Application::Stop()
{
	if (m_captureProcessorThread)
	{
		if (m_captureProcessorThread->IsRunning())
		{
			m_captureProcessorThread->Delete();
			m_captureProcessorThread->Wait();
		}
		delete m_captureProcessorThread;
		m_captureProcessorThread = nullptr;
	}

	m_frameQueue.NotifyAll();

	if (m_frameProcessorThread)
	{
		if (m_frameProcessorThread->IsRunning())
		{
			m_frameProcessorThread->Delete();
			m_frameProcessorThread->Wait();
		}
		delete m_frameProcessorThread;
		m_frameProcessorThread = nullptr;
	}

	m_packetQueue.NotifyAll();

	if (m_networkThread)
	{
		if (m_networkThread->IsRunning())
		{
			m_networkThread->Delete();
			m_networkThread->Wait();
		}
		delete m_networkThread;
		m_networkThread = nullptr;
	}
}

