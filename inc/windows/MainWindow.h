#pragma once
#include <wx/frame.h>
#include <wx/notebook.h>
#include <wx/menu.h>
#include <wx/sizer.h>
#include "tabs/ConnectTab.h"
#include "tabs/GeneralTab.h"
#include "tabs/InputTab.h"
#include "tabs/SensorsTab.h"
#include "tabs/BaseStationTab.h"
#include "tabs/VRDriverTab.h"

class MainWindow : public wxFrame
{
public:
	MainWindow();
	~MainWindow();

private:
	wxNotebook* tabs = nullptr;

	wxNotebookPage* connectTab     = nullptr; // Settings of network
	wxNotebookPage* generalTab     = nullptr; // General settings setup encoders, video stream, lens correction, pad-width, preview
	wxNotebookPage* inputTab       = nullptr; // Setup for input from sensors: Sensor touch + base station gestures
	wxNotebookPage* sensorsTab     = nullptr; // calibrate phone sensor
	wxNotebookPage* baseStationTab = nullptr; // calibrate base station detection
	wxNotebookPage* vrDriverTab    = nullptr; // settings driver relation configs

	void OnClose(wxCloseEvent& event);
};

