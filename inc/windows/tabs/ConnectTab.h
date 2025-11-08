#pragma once
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/button.h>

class ConnectTab : public wxPanel
{
public:
	ConnectTab(wxWindow* parent);
	~ConnectTab();

private:
	enum
	{
		CONNECT_BUTTON_ID = wxID_HIGHEST + 1
	};

	wxTextCtrl* ipAddressTxt = nullptr;
	wxTextCtrl* controlPortTxt = nullptr;
	wxTextCtrl* videoStreamPortTxt = nullptr;

	wxStaticText* infoPCIP = nullptr;
	wxStaticText* infoPCName = nullptr;

	wxStaticText* deviceIP = nullptr;
	wxStaticText* deviceResilution = nullptr;
	wxStaticText* deviceAspectRatio = nullptr;
	wxStaticText* deviceBataryLevel = nullptr;

	wxButton* connectBtn = nullptr;

	void OnConnectButtonClk(wxCommandEvent& event);
};