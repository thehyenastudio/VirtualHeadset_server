#include <windows/tabs/InputTab.h>

InputTab::InputTab(wxWindow* parent) : wxPanel(parent, wxID_ANY)
{
	wxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(new wxStaticText(this, wxID_ANY, "UNDER DEVELOPMENT", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL), 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	this->SetSizer(mainSizer);
}

InputTab::~InputTab()
{
}
