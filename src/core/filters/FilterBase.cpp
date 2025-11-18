#include "core/filters/FilterBase.h"

void FilterBase::Destroy()
{
	if (m_outFrame)
	{
		av_frame_free(&m_outFrame);
		m_outFrame = nullptr;
	}
	if (m_graph)
	{
		avfilter_graph_free(&m_graph);
		m_graph = nullptr;
	}
	m_buffersrc = nullptr;
	m_buffersink = nullptr;
}

AVFilterContext* FilterBase::CreateFilter(const char* name, const char* instName, const char* args)
{
	const AVFilter* f = avfilter_get_by_name(name);
	if (!f)
	{
		wxLogError("Filter not faund: %s", name);
	}
	AVFilterContext* ctx = nullptr;
	int ret = avfilter_graph_create_filter(&ctx, f, instName, args, nullptr, m_graph);
	if (ret < 0)
	{
		wxLogError("Failed to create filterd: %s", name);
	}
	return ctx;
}

void FilterBase::LinkFilter(AVFilterContext* src, int srcPad, AVFilterContext* dst, int dstPad)
{
	int ret = avfilter_link(src, srcPad, dst, dstPad);
	if (ret < 0)
	{
		wxLogError("Failed to link filters");
	}
}
