#include "core/WindowManager.h"
#include "windows/PreviewWindow.h"

wxDEFINE_EVENT(EVT_NEW_FRAME, wxCommandEvent);

PreviewWindow::PreviewWindow() : wxFrame(nullptr, wxID_ANY, "Preview")
{
	this->SetIcon(wxIcon("icon.ico", wxBITMAP_TYPE_ICO));
	this->SetSize(480, 320);

	this->Bind(EVT_NEW_FRAME, &PreviewWindow::OnReceivedFrame, this);
	this->Bind(wxEVT_CLOSE_WINDOW, &PreviewWindow::OnClose, this);
}

PreviewWindow::~PreviewWindow()
{
	this->Unbind(EVT_NEW_FRAME, &PreviewWindow::OnReceivedFrame, this);
	this->Unbind(wxEVT_CLOSE_WINDOW, &PreviewWindow::OnClose, this);
}

void PreviewWindow::OnReceivedFrame(wxCommandEvent& event)
{
	AVFrame* rawFrame = static_cast<AVFrame*>(event.GetClientData());
	if (!rawFrame) return;
	int width = rawFrame->width;
	int height = rawFrame->height;
	if (width <= 0 || height <= 0) return;

	// Prepare sws context to convert from source pixel format to RGB24
	AVPixelFormat srcFmt = static_cast<AVPixelFormat>(rawFrame->format);
	AVPixelFormat dstFmt = AV_PIX_FMT_RGB24;
	struct SwsContext* swsCtx = sws_getContext(
		width, height, srcFmt,
		width, height, dstFmt,
		SWS_BILINEAR, nullptr, nullptr, nullptr);
	if (!swsCtx) return;

	// Allocate buffer for RGB24 image
	int rgbLinesize = 0;
	int rgbBufSize = av_image_get_buffer_size(dstFmt, width, height, 1);
	if (rgbBufSize <= 0)
	{
		sws_freeContext(swsCtx);
		return;
	}
	uint8_t* rgbBuffer = (uint8_t*) av_malloc(rgbBufSize);
	if (!rgbBuffer)
	{
		sws_freeContext(swsCtx);
		return;
	}

	uint8_t* dstData[4] = { nullptr };
	int dstLinesize[4] = { 0 };
	av_image_fill_arrays(dstData, dstLinesize, rgbBuffer, dstFmt, width, height, 1);

	// Convert
	sws_scale(swsCtx, rawFrame->data, rawFrame->linesize, 0, height, dstData, dstLinesize);

	// Create wxImage (wxImage will copy the data when constructed with static_data=false)
	wxImage image;
	image.Create(width, height);
	// wxImage::SetData expects RGB interleaved data, which we have.
	image.SetData(reinterpret_cast<unsigned char*>(rgbBuffer), rgbBufSize);

	// Cleanup libswscale allocation

	// Create bitmap and draw it into the window
	wxBitmap bmp(image);

	av_free(rgbBuffer);
	sws_freeContext(swsCtx);

	if (bmp.IsOk())
	{
		wxClientDC dc(this);
		// Optionally scale to client size; here we draw at top-left with image native size
		dc.DrawBitmap(bmp, 0, 0, false);
		this->Refresh(false);
	}

	av_frame_free(&rawFrame);
}

void PreviewWindow::OnClose(wxCloseEvent& event)
{
	WindowManager::Instance().LoseMainWindow();
	event.Skip();
}
