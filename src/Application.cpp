#include <Application.h>
#include <config.h>
#include <fstream>

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

		m_captureProcessorThread = new CaptureProcessorThread();
		m_captureProcessorThread->Create();
		m_captureProcessorThread->Run(); // TODO: run if needed (connected or preview shown)

		m_tray = new Tray();

		if (!Config::Instance().GetStartMinimized())
		{
			WindowManager::Instance().GetMainWindow()->Show();
		}

		wxLogMessage(_("Virtual Headset server started"));
		return true;
	}
	else 
	{
		return false;
	}
}

int Application::OnExit()
{
	if (m_captureProcessorThread)
	{
		m_captureProcessorThread->Delete();
	}

	Config::Instance().Save(); // TODO: ONLY TEST
	wxLog::SetActiveTarget(nullptr);
	if (m_logStream) {
		m_logStream->close();
		delete m_logStream;
		m_logStream = nullptr;
	}
	return 0;
}
