#pragma once
#include <wx/menu.h>
#include <wx/taskbar.h>

#include "core/WindowManager.h"

class Tray : public wxTaskBarIcon {
public:
	Tray();
	~Tray();

private:
	enum
	{
		SHOW_BUTTON_ID = wxID_HIGHEST + 1,
		SHOW_PREVIEW_BUTTON_ID,
		EXIT_BUTTON_ID = wxID_EXIT
	};

	virtual wxMenu* CreatePopupMenu() override;

	// =========== EVENTS ===========
	void OnLeftButtonDClick(wxTaskBarIconEvent&);

	void OnShowBtn(wxCommandEvent&);
	void OnPreviewBtn(wxCommandEvent&);
	void OnExitBtn(wxCommandEvent&);
};