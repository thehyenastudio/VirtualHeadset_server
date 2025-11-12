#pragma once
#include <wx/dcclient.h>
#include <wx/event.h>
#include <wx/frame.h>

extern "C" {
#include <libavutil/frame.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}


wxDECLARE_EVENT(EVT_NEW_FRAME, wxCommandEvent);

class PreviewWindow : public wxFrame
{
public:
	PreviewWindow();
	~PreviewWindow();

private:
	void OnReceivedFrame(wxCommandEvent& event);
	void OnClose(wxCloseEvent& event);
};