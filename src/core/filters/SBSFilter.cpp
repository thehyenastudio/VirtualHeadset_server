#include "core/filters/SBSFilter.h"

SBSFilter::SBSFilter(int deviceW, int deviceH, int gap, AVPixelFormat fmt, AVRational tb)
{
	m_deviceW = deviceW;
	m_deviceH = deviceH;
	m_gap = gap;
	m_fmt = fmt;
	m_tb = tb;
}

void SBSFilter::Init(bool useGPU)
{
	if (useGPU)
	{
		wxLogWarning("GPU not implemented yet...");
	}

	Destroy();
	m_graph = avfilter_graph_alloc();
	if (!m_graph)
	{
		wxLogError("Failed to allocate filter graph");
		return;
	}

	// Common args for buffer sources (half width inputs)
	const int halfW = (m_deviceW - m_gap) / 2;

	char args[128];
	snprintf(args, sizeof(args), "video_size=%dx%d:pix_fmt=%d:time_base=%d/%d:pixel_aspect=1/1:frame_rate=%d/%d", halfW, m_deviceH, m_fmt, m_tb.num, m_tb.den, m_tb.den, m_tb.num);
	m_leftSrc = CreateFilter("buffer", "left_in", args);
	if (!m_leftSrc) return;

	snprintf(args, sizeof(args), "video_size=%dx%d:pix_fmt=%d:time_base=%d/%d:pixel_aspect=1/1:frame_rate=%d/%d", halfW, m_deviceH, m_fmt, m_tb.num, m_tb.den, m_tb.den, m_tb.num);
	m_rightSrc = CreateFilter("buffer", "right_in", args);
	if (!m_rightSrc) return;

	// Force both inputs to yuv420p (overlay/pad are most stable here)
	AVFilterContext* leftFmt = CreateFilter("format", "left_fmt", "pix_fmts=yuv420p");
	AVFilterContext* rightFmt = CreateFilter("format", "right_fmt", "pix_fmts=yuv420p");
	if (!leftFmt || !rightFmt) return;

	// Pad the left input to full device size, keep content at x=0 (gap will be between halves)
	char padArgs[64];
	snprintf(padArgs, sizeof(padArgs), "width=%d:height=%d:x=0:y=0:color=black", m_deviceW, m_deviceH);
	AVFilterContext* pad = CreateFilter("pad", "pad", padArgs);
	if (!pad) return;

	// Overlay right eye onto padded left at X = halfW + gap
	char overlayArgs[64];
	snprintf(overlayArgs, sizeof(overlayArgs), "%d:0", halfW + m_gap);
	AVFilterContext* overlay = CreateFilter("overlay", "overlay", overlayArgs);
	if (!overlay) return;

	// Convert final overlay to NV12 (desired output)
	AVFilterContext* outFmt = CreateFilter("format", "out_fmt", "pix_fmts=bgra");
	if (!outFmt) return;

	m_buffersink = CreateFilter("buffersink", "out", nullptr);
	if (!m_buffersink) return;

	// Link: left_src → left_fmt → pad (base)
	LinkFilter(m_leftSrc, 0, leftFmt, 0);
	LinkFilter(leftFmt, 0, pad, 0);

	// Link: right_src → right_fmt → overlay (overlay input)
	LinkFilter(m_rightSrc, 0, rightFmt, 0);
	LinkFilter(rightFmt, 0, overlay, 1);

	// Link: pad (base) → overlay → out_fmt → sink
	LinkFilter(pad, 0, overlay, 0);
	LinkFilter(overlay, 0, outFmt, 0);
	LinkFilter(outFmt, 0, m_buffersink, 0);

	int cfg = avfilter_graph_config(m_graph, nullptr);
	if (cfg < 0)
	{
		wxLogError("avfilter_graph_config failed: %d", cfg);
		return;
	}

	m_outFrame = av_frame_alloc();
	if (!m_outFrame)
	{
		wxLogError("Failed to allocate outFrame");
		return;
	}
}

AVFrame* SBSFilter::Filter(const AVFrame* left, const AVFrame* right)
{
	if (!m_graph || !m_leftSrc || !m_rightSrc || !m_buffersink || !m_outFrame)
		return nullptr;

	// Push left (keep ref to avoid premature free)
	int ret = av_buffersrc_add_frame_flags(
		m_leftSrc, const_cast<AVFrame*>(left), AV_BUFFERSRC_FLAG_KEEP_REF);
	if (ret < 0)
	{
		wxLogError("left buffersrc_add_frame failed: %d", ret);
		return nullptr;
	}

	// Push right
	ret = av_buffersrc_add_frame_flags(
		m_rightSrc, const_cast<AVFrame*>(right), AV_BUFFERSRC_FLAG_KEEP_REF);
	if (ret < 0)
	{
		wxLogError("right buffersrc_add_frame failed: %d", ret);
		return nullptr;
	}

	// Pull combined
	av_frame_unref(m_outFrame);
	ret = av_buffersink_get_frame(m_buffersink, m_outFrame);
	if (ret < 0)
	{
		wxLogError("buffersink_get_frame failed: %d", ret);
		return nullptr;
	}
	return m_outFrame;
}
