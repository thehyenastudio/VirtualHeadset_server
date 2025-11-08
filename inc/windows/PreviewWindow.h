#pragma once
#include <wx/frame.h>

class PreviewWindow : public wxFrame
{
public:
	PreviewWindow();
	~PreviewWindow();

private:
	void OnClose(wxCloseEvent& event);
};