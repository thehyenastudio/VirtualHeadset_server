#include "core/WindowManager.h"
#include "windows/PreviewWindow.h"

wxDEFINE_EVENT(EVT_NEW_FRAME, wxCommandEvent);

PreviewWindow::PreviewWindow() : wxFrame(nullptr, wxID_ANY, "Preview")
{
	this->SetIcon(wxIcon("icon.ico", wxBITMAP_TYPE_ICO));
	this->SetSize(480, 320);

	this->SetBackgroundStyle(wxBG_STYLE_PAINT);

	this->Bind(EVT_NEW_FRAME, &PreviewWindow::OnReceivedFrame, this);
	this->Bind(wxEVT_SIZE, &PreviewWindow::OnResize, this);
	this->Bind(wxEVT_PAINT, &PreviewWindow::OnPaint, this);
	this->Bind(wxEVT_ERASE_BACKGROUND, &PreviewWindow::OnErase, this);
	this->Bind(wxEVT_CLOSE_WINDOW, &PreviewWindow::OnClose, this);
}

PreviewWindow::~PreviewWindow()
{
	this->Unbind(EVT_NEW_FRAME, &PreviewWindow::OnReceivedFrame, this);
	this->Unbind(wxEVT_SIZE, &PreviewWindow::OnResize, this);
	this->Unbind(wxEVT_PAINT, &PreviewWindow::OnPaint, this);
	this->Unbind(wxEVT_ERASE_BACKGROUND, &PreviewWindow::OnErase, this);
	this->Unbind(wxEVT_CLOSE_WINDOW, &PreviewWindow::OnClose, this);

	if (m_swsCtx)
	{
		sws_freeContext(m_swsCtx);
		m_swsCtx = nullptr;
	}
}

wxBitmap PreviewWindow::AVFrameToBitmap(AVFrame* frame)
{
	if (!frame || frame->width <= 0 || frame->height <= 0 || frame->data[0] == nullptr)
	{
		return wxNullBitmap;
	}

	AVPixelFormat srcFmt = static_cast<AVPixelFormat>(frame->format);
	AVPixelFormat dstFmt = AV_PIX_FMT_RGB24;

	if (!m_swsCtx)
	{
		m_swsCtx = sws_getContext(
			frame->width, frame->height, srcFmt,
			frame->width, frame->height, dstFmt,
			SWS_BILINEAR, nullptr, nullptr, nullptr
		);
		if (!m_swsCtx)
		{
			return wxNullBitmap;
		}
	}

	// Allocate buffer with malloc (compatible with wxImage)
	int rgbStride = frame->width * 3;
	int rgbBufferSize = rgbStride * frame->height;
	if (m_rgbBuffer.size() < static_cast<size_t>(rgbBufferSize))
	{
		m_rgbBuffer.resize(rgbBufferSize);
	}

	uint8_t* rgbData[4] = { m_rgbBuffer.data(), nullptr, nullptr, nullptr };
	int rgbStrides[4] = { rgbStride, 0, 0, 0 };

	int result = sws_scale(m_swsCtx, frame->data, frame->linesize, 0, frame->height, rgbData, rgbStrides);

	if (result <= 0)
	{
		return wxNullBitmap;
	}

	// Create wxImage that takes ownership of the buffer
	wxImage img(frame->width, frame->height, m_rgbBuffer.data(), true);
	if (!img.IsOk())
	{
		return wxNullBitmap;
	}

	return wxBitmap(img);
}

void PreviewWindow::OnReceivedFrame(wxCommandEvent& event)
{
	AVFrame* frame = static_cast<AVFrame*>(event.GetClientData());

	if (frame)
	{
		wxBitmap newFrame = AVFrameToBitmap(frame);
		if (newFrame.IsOk())
		{
			m_frame = std::move(newFrame);
			Refresh();
		}
		av_frame_free(&frame);
	}
}

void PreviewWindow::OnResize(wxSizeEvent& event)
{
	Refresh();
	event.Skip();
}

void PreviewWindow::OnPaint(wxPaintEvent& event)
{
	wxAutoBufferedPaintDC dc(this);

	wxSize client = GetClientSize();
	int cw = client.GetWidth();
	int ch = client.GetHeight();

	dc.SetBrush(*wxBLACK_BRUSH);
	dc.SetPen(*wxTRANSPARENT_PEN);
	dc.DrawRectangle(0, 0, cw, ch);

	if (!m_frame.IsOk())
	{
		dc.SetTextForeground(*wxWHITE);
		dc.DrawText("Frame not founded...", cw / 2 - 40, ch / 2 - 10);
		return;
	}

	int bw = m_frame.GetWidth();
	int bh = m_frame.GetHeight();

	if (bw <= 0 || bh <= 0)
	{
		return;
	}

	double sx = static_cast<double>(cw) / bw;
	double sy = static_cast<double>(ch) / bh;
	double scale = std::min(sx, sy);

	int dw = static_cast<int>(bw * scale);
	int dh = static_cast<int>(bh * scale);
	int dx = (cw - dw) / 2;
	int dy = (ch - dh) / 2;

	// Try using graphics context first, fallback to regular DC
	std::unique_ptr<wxGraphicsContext> gc(wxGraphicsContext::Create(dc));
	if (gc)
	{
		gc->SetInterpolationQuality(wxINTERPOLATION_FAST);
		gc->DrawBitmap(m_frame, dx, dy, dw, dh);
	}
	else
	{
		// Simple DC drawing
		wxMemoryDC memDC;
		memDC.SelectObject(m_frame);
		dc.StretchBlit(dx, dy, dw, dh, &memDC, 0, 0, bw, bh);
		memDC.SelectObject(wxNullBitmap);
	}
}

void PreviewWindow::OnErase(wxEraseEvent& event)
{}

void PreviewWindow::OnClose(wxCloseEvent& event)
{
	WindowManager::Instance().LosePreviewWindow();
	event.Skip();
}
