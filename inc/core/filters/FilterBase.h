#pragma once
extern "C" {
#include <libavfilter/avfilter.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libavutil/frame.h>
}

class FilterBase
{
public:
	FilterBase() = default;
	virtual ~FilterBase() { Destroy(); }

	virtual void Init() = 0;
	virtual AVFrame* Filter(const AVFrame* in) = 0;

	virtual void Destroy();

protected:
	AVFilterGraph* m_graph = nullptr;
	AVFilterContext* m_buffersrc = nullptr;
	AVFilterContext* m_buffersink = nullptr;
	AVFrame* m_outFrame = nullptr;
};