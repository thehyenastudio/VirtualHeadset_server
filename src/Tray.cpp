#include <Tray.h>

Tray::Tray()
{
	this->SetIcon(wxIcon("icon.ico", wxBITMAP_TYPE_ICO));

	this->Bind(wxEVT_TASKBAR_LEFT_DCLICK, &Tray::OnLeftButtonDClick, this);
	this->Bind(wxEVT_MENU, &Tray::OnShowBtn, this, SHOW_BUTTON_ID);
	this->Bind(wxEVT_MENU, &Tray::OnPreviewBtn, this, SHOW_PREVIEW_BUTTON_ID);
	this->Bind(wxEVT_MENU, &Tray::OnExitBtn, this, EXIT_BUTTON_ID);
}

Tray::~Tray()
{
	this->Unbind(wxEVT_TASKBAR_LEFT_DCLICK, &Tray::OnLeftButtonDClick, this);
	this->Unbind(wxEVT_MENU, &Tray::OnShowBtn, this, SHOW_BUTTON_ID);
	this->Unbind(wxEVT_MENU, &Tray::OnPreviewBtn, this, SHOW_PREVIEW_BUTTON_ID);
	this->Unbind(wxEVT_MENU, &Tray::OnExitBtn, this, EXIT_BUTTON_ID);
}

wxMenu* Tray::CreatePopupMenu()
{
	wxMenu* menu = new wxMenu;
	menu->Append(SHOW_BUTTON_ID, _("Show "));
	menu->Append(SHOW_PREVIEW_BUTTON_ID, _("Show preview"));
	menu->AppendSeparator();
	menu->Append(EXIT_BUTTON_ID, _("Exit"));
	return menu;
}

void Tray::OnLeftButtonDClick(wxTaskBarIconEvent&)
{
	WindowManager::Instance().GetMainWindow()->Show();
}

void Tray::OnShowBtn(wxCommandEvent&)
{
	WindowManager::Instance().GetMainWindow()->Show();
}

void Tray::OnPreviewBtn(wxCommandEvent&)
{
	WindowManager::Instance().GetPreviewWindow()->Show();
}

void Tray::OnExitBtn(wxCommandEvent&)
{
	wxTheApp->Exit();
}
