#include "core/filters/EyeFilter.h"

EyeFilter::EyeFilter(int srcW, int srcH, AVPixelFormat srcFmt, AVRational tb, const EyeParams& params)
{
	m_srcW = srcW;
	m_srcH = srcH;
	m_srcFmt = srcFmt;
	m_tb = tb;
	m_params = params;
}

void EyeFilter::Init(bool useGPU)
{
	if (useGPU)
	{
		wxLogWarning("GPU not implemented yet...");
	}

	this->Destroy();
	m_graph = avfilter_graph_alloc();

	char args[128];
	snprintf(args, sizeof(args), "video_size=%dx%d:pix_fmt=%d:time_base=%d/%d:pixel_aspect=1/1:frame_rate=%d/%d", m_srcW, m_srcH, m_srcFmt, m_tb.num, m_tb.den, m_tb.den, m_tb.num);

	m_buffersrc = CreateFilter("buffer", "in", args);
	m_buffersink = CreateFilter("buffersink", "out", nullptr);

	char scaleArgs[64];
	snprintf(scaleArgs, sizeof(scaleArgs), "%d:%d", m_params.targetW, m_params.targetH);
	AVFilterContext* scale = CreateFilter("scale", "scale", scaleArgs);

	char cropArgs[64];
	snprintf(cropArgs, sizeof(cropArgs), "%d:%d:%d:%d", m_params.cropW, m_params.cropH, m_params.cropX, m_params.cropY);
	AVFilterContext* crop = CreateFilter("crop", "crop", cropArgs);

	char lensArgs[64];
	snprintf(lensArgs, sizeof(lensArgs), "k1=%f:k2=%f", m_params.k1, m_params.k2);
	AVFilterContext* lens = CreateFilter("lenscorrection", "lens", lensArgs);

	AVFilterContext* outFmt = CreateFilter("format", "out_fmt", "pix_fmts=bgra");

	this->LinkFilter(m_buffersrc, 0, scale, 0);
	this->LinkFilter(scale, 0, crop, 0);
	this->LinkFilter(crop, 0, lens, 0);
	this->LinkFilter(lens, 0, outFmt, 0);
	this->LinkFilter(outFmt, 0, m_buffersink, 0);

	avfilter_graph_config(m_graph, nullptr);
	m_outFrame = av_frame_alloc();
}

AVFrame* EyeFilter::Filter(const AVFrame* in)
{
	AVFrame* tmp = av_frame_clone(const_cast<AVFrame*>(in));
	av_buffersrc_add_frame(m_buffersrc, tmp);
	av_frame_free(&tmp);

	av_frame_unref(m_outFrame);
	av_buffersink_get_frame(m_buffersink, m_outFrame);
	return m_outFrame;
}
