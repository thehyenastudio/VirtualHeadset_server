#pragma once
#include <wx/log.h>

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

	virtual void Init(bool useGPU = false) = 0;
	virtual AVFrame* Filter(const AVFrame* in) = 0;
	virtual AVFrame* Filter(const AVFrame* left, const AVFrame* right) = 0;

	virtual void Destroy();

protected:
	AVFilterGraph* m_graph = nullptr;
	AVFilterContext* m_buffersrc = nullptr;
	AVFilterContext* m_buffersink = nullptr;
	AVFrame* m_outFrame = nullptr;

	AVFilterContext* CreateFilter(const char* name, const char* instName, const char* args);
	void LinkFilter(AVFilterContext* src, int srcPad, AVFilterContext* dst, int dstPad);
};