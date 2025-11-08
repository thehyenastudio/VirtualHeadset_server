#include <windows/PreviewWindow.h>
#include <core/WindowManager.h>

PreviewWindow::PreviewWindow() : wxFrame(nullptr, wxID_ANY, "Preview")
{
	this->Bind(wxEVT_CLOSE_WINDOW, &PreviewWindow::OnClose, this);
}

PreviewWindow::~PreviewWindow()
{
	this->Unbind(wxEVT_CLOSE_WINDOW, &PreviewWindow::OnClose, this);
}

void PreviewWindow::OnClose(wxCloseEvent& event)
{
	WindowManager::Instance().LoseMainWindow();
	event.Skip();
}
