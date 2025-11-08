#include <windows/tabs/GeneralTab.h>

GeneralTab::GeneralTab(wxWindow* parent) : wxPanel(parent, wxID_ANY)
{
	wxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(mainSizer);

	wxSizer* frameSettingsSizer = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(frameSettingsSizer, 1, wxEXPAND | wxALL);

	wxFlexGridSizer* frameSettingsColumn1Sizer = new wxFlexGridSizer(3);
	frameSettingsColumn1Sizer->AddGrowableCol(0);
	frameSettingsSizer->Add(frameSettingsColumn1Sizer, 1, wxEXPAND | wxLEFT, 5);
	frameSettingsColumn1Sizer->Add(new wxStaticText(this, wxID_ANY, _("Eye Resolution:")), 0, wxALL, 5);
	eyeWidthResolutionTxt = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTER);
	frameSettingsColumn1Sizer->Add(eyeWidthResolutionTxt, 0, wxALL | wxEXPAND, 5);
	eyeHeightResolutionTxt = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTER);
	frameSettingsColumn1Sizer->Add(eyeHeightResolutionTxt, 0, wxALL | wxEXPAND, 5);
	useSquareResolutionBtn = new wxToggleButton(this, wxID_ANY, _("Use Square Resolution"));
	frameSettingsColumn1Sizer->Add(useSquareResolutionBtn, 0, wxALL | wxEXPAND, 5);
	frameSettingsColumn1Sizer->Add(new wxStaticText(this, wxID_ANY, _("FPS:")), 0, wxALL, 5);
	eyeFPSTxt = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTER);
	frameSettingsColumn1Sizer->Add(eyeFPSTxt, 0, wxALL | wxEXPAND, 5);
	frameSettingsColumn1Sizer->Add(new wxStaticText(this, wxID_ANY, _("GAP:")), 0, wxALL, 5);
	gapSizeSlider = new wxSlider(this, wxID_ANY, 0, 0, 25, wxDefaultPosition, wxDefaultSize, wxSL_BOTH | wxSL_SELRANGE);
	frameSettingsColumn1Sizer->Add(gapSizeSlider, 0, wxALL | wxEXPAND, 5);
	gapSizeTxt = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTER);
	frameSettingsColumn1Sizer->Add(gapSizeTxt, 0, wxALL | wxEXPAND, 5);
	frameSettingsColumn1Sizer->Add(new wxStaticText(this, wxID_ANY, _("Scale Factor:")), 0, wxALL, 5);
	scaleFactorSlider = new wxSlider(this, wxID_ANY, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_BOTH | wxSL_SELRANGE);
	frameSettingsColumn1Sizer->Add(scaleFactorSlider, 0, wxALL | wxEXPAND, 5);
	scaleFactorTxt = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTER);
	frameSettingsColumn1Sizer->Add(scaleFactorTxt, 0, wxALL | wxEXPAND, 5);
	frameSettingsColumn1Sizer->Add(new wxStaticText(this, wxID_ANY, _("Crop Factor:")), 0, wxALL, 5);
	cropFactorSlider = new wxSlider(this, wxID_ANY, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_BOTH | wxSL_SELRANGE);
	frameSettingsColumn1Sizer->Add(cropFactorSlider, 0, wxALL | wxEXPAND, 5);
	cropFactorTxt = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTER);
	frameSettingsColumn1Sizer->Add(cropFactorTxt, 0, wxALL | wxEXPAND, 5);


	wxSizer* frameSettingsColumn2Sizer = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("Preview")), wxVERTICAL);;
	frameSettingsSizer->Add(frameSettingsColumn2Sizer, 0, wxRIGHT, 5);


	wxSizer* fake3DSettingsSizer = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(fake3DSettingsSizer, 0, wxEXPAND | wxALL);
}

GeneralTab::~GeneralTab()
{
}