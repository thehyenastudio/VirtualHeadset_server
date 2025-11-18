#pragma once
#include <wx/dcbuffer.h>
#include <wx/dcclient.h>
#include <wx/event.h>
#include <wx/frame.h>
#include <wx/graphics.h> 

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
	wxBitmap m_frame;
	SwsContext* m_swsCtx = nullptr;
	std::vector<uint8_t> m_rgbBuffer;

	wxBitmap AVFrameToBitmap(AVFrame* frame);

	void OnReceivedFrame(wxCommandEvent& event);
	void OnResize(wxSizeEvent& event);
	void OnPaint(wxPaintEvent& event);
	void OnErase(wxEraseEvent& event);
	void OnClose(wxCloseEvent& event);
};