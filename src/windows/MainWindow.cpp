#include <windows/MainWindow.h>
#include <core/WindowManager.h>
#include <config.h>

MainWindow::MainWindow() : wxFrame(nullptr, wxID_ANY, Config::APP_NAME)
{
	this->SetIcon(wxIcon("icon.ico", wxBITMAP_TYPE_ICO));

	this->SetSize(480, 320);

	wxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(mainSizer);

	tabs = new wxNotebook(this, wxID_ANY);
	mainSizer->Add(tabs, 1, wxEXPAND | wxALL);

	connectTab = new ConnectTab(tabs);
	tabs->AddPage(connectTab, "Connect");
	
	generalTab = new GeneralTab(tabs);
	tabs->AddPage(generalTab, "General");

	inputTab = new InputTab(tabs);
	tabs->AddPage(inputTab, "Input");

	sensorsTab = new SensorsTab(tabs);
	tabs->AddPage(sensorsTab, "Sensors");

	baseStationTab = new BaseStationTab(tabs);
	tabs->AddPage(baseStationTab, "Base Station");

	vrDriverTab = new VRDriverTab(tabs);
	tabs->AddPage(vrDriverTab, "VR Driver");

	this->CreateStatusBar(2);
	this->GetStatusBar()->SetStatusText("Ready", 0);
	this->GetStatusBar()->SetStatusText("Device not connected...", 1);

	this->Bind(wxEVT_CLOSE_WINDOW, &MainWindow::OnClose, this);
}

MainWindow::~MainWindow()
{
	this->Unbind(wxEVT_CLOSE_WINDOW, &MainWindow::OnClose, this);
}

void MainWindow::OnClose(wxCloseEvent& event)
{
	WindowManager::Instance().LoseMainWindow();
	event.Skip();
}
