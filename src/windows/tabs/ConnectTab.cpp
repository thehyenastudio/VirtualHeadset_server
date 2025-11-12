#include "config.h"
#include "windows/tabs/ConnectTab.h"

ConnectTab::ConnectTab(wxWindow* parent) : wxPanel(parent, wxID_ANY)
{
	wxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(mainSizer);

	wxSizer* networkSettingsSizer = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(networkSettingsSizer, 1, wxEXPAND | wxALL);

	wxSizer* networkSettingsColumn1Sizer = new wxFlexGridSizer(2);
	networkSettingsSizer->Add(networkSettingsColumn1Sizer, 1, wxEXPAND | wxLEFT, 5);
	networkSettingsColumn1Sizer->Add(new wxStaticText(this, wxID_ANY, "IP Address:"), 0, wxALIGN_CENTER_VERTICAL | wxALL);
	ipAddressTxt = new wxTextCtrl(this, wxID_ANY, Config::Instance().GetPcIpAddress(), wxDefaultPosition, wxDefaultSize, wxTE_CENTER);
	networkSettingsColumn1Sizer->Add(ipAddressTxt, 0, wxEXPAND | wxALL);
	networkSettingsColumn1Sizer->Add(new wxStaticText(this, wxID_ANY, "Control Port:"), 0, wxALIGN_CENTER_VERTICAL | wxALL);
	controlPortTxt = new wxTextCtrl(this, wxID_ANY, wxString::Format("%d", Config::Instance().GetControlPort()), wxDefaultPosition, wxDefaultSize, wxTE_CENTER);
	networkSettingsColumn1Sizer->Add(controlPortTxt, 0, wxEXPAND | wxALL);
	networkSettingsColumn1Sizer->Add(new wxStaticText(this, wxID_ANY, "Video Stream Port:"), 0, wxALIGN_CENTER_VERTICAL | wxALL);
	videoStreamPortTxt = new wxTextCtrl(this, wxID_ANY, wxString::Format("%d", Config::Instance().GetVideoStreamPort()), wxDefaultPosition, wxDefaultSize, wxTE_CENTER);
	networkSettingsColumn1Sizer->Add(videoStreamPortTxt, 0, wxEXPAND | wxALL);

	wxSizer* networkSettingsColumn2Sizer = new wxBoxSizer(wxVERTICAL);
	networkSettingsSizer->Add(networkSettingsColumn2Sizer, 0, wxEXPAND | wxRIGHT, 5);

	wxSizer* pcInfoSizer = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("PC info")), wxVERTICAL);
	networkSettingsColumn2Sizer->Add(pcInfoSizer, 0, wxEXPAND | wxALL);
	wxFlexGridSizer* pcSizer = new wxFlexGridSizer(2, 0, 5);
	pcSizer->AddGrowableCol(0);
	pcInfoSizer->Add(pcSizer, 0, wxEXPAND | wxALL);
	pcSizer->Add(new wxStaticText(this, wxID_ANY, "IP:"), 0, wxALIGN_CENTER_VERTICAL | wxALL);
	infoPCIP = new wxStaticText(this, wxID_ANY, "N/A");
	pcSizer->Add(infoPCIP, 0, wxEXPAND | wxALL);
	pcSizer->Add(new wxStaticText(this, wxID_ANY, "Name:"), 0, wxALIGN_CENTER_VERTICAL | wxALL);
	infoPCName = new wxStaticText(this, wxID_ANY, "N/A");
	pcSizer->Add(infoPCName, 0, wxEXPAND | wxALL);

	wxSizer* deviceInfoSizer = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("Device info")), wxVERTICAL);
	networkSettingsColumn2Sizer->Add(deviceInfoSizer, 0, wxEXPAND | wxALL);
	wxFlexGridSizer* deviceSizer = new wxFlexGridSizer(2, 0, 5);
	deviceSizer->AddGrowableCol(0);
	deviceInfoSizer->Add(deviceSizer, 0, wxEXPAND | wxALL);
	deviceSizer->Add(new wxStaticText(this, wxID_ANY, "IP:"), 0, wxEXPAND | wxALL);
	deviceIP = new wxStaticText(this, wxID_ANY, "N/A");
	deviceSizer->Add(deviceIP, 0, wxEXPAND | wxALL);
	deviceSizer->Add(new wxStaticText(this, wxID_ANY, "Resolution:"), 0, wxEXPAND | wxALL);
	deviceResilution = new wxStaticText(this, wxID_ANY, "N/A");
	deviceSizer->Add(deviceResilution, 0, wxEXPAND | wxALL);
	deviceSizer->Add(new wxStaticText(this, wxID_ANY, "Aspect Ratio:"), 0, wxEXPAND | wxALL);
	deviceAspectRatio = new wxStaticText(this, wxID_ANY, "N/A");
	deviceSizer->Add(deviceAspectRatio, 0, wxEXPAND | wxALL);
	deviceSizer->Add(new wxStaticText(this, wxID_ANY, "Battery Level:"), 0, wxEXPAND | wxALL);
	deviceBataryLevel = new wxStaticText(this, wxID_ANY, "N/A");
	deviceSizer->Add(deviceBataryLevel, 0, wxEXPAND | wxALL);

	wxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(buttonSizer, 0, wxEXPAND | wxALL);

	connectBtn = new wxButton(this, CONNECT_BUTTON_ID, "Connect");
	buttonSizer->Add(connectBtn, 0, wxEXPAND | wxALL);

	connectBtn->Bind(wxEVT_BUTTON, &ConnectTab::OnConnectButtonClk, this);
}

ConnectTab::~ConnectTab()
{
	connectBtn->Unbind(wxEVT_BUTTON, &ConnectTab::OnConnectButtonClk, this);
}

void ConnectTab::OnConnectButtonClk(wxCommandEvent& event)
{

}
