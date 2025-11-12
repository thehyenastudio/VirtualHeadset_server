#include "core/filters/FFmpegFilter.h"

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