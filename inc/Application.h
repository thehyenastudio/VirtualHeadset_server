#pragma once
#include <wx/app.h>
#include "Tray.h"
#include "core/WindowManager.h"
#include "core/threads/CaptureProcessorThread.h"

class Application : public wxApp
{
public:
	virtual bool OnInit() override;
	virtual int OnExit() override;

private:
	std::ofstream* m_logStream = nullptr;
	Tray* m_tray = nullptr;

	CaptureProcessorThread* m_captureProcessorThread = nullptr;
};