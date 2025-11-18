#pragma once
#include "FilterBase.h"

class SBSFilter : public FilterBase
{
public:
	SBSFilter(int deviceW, int deviceH, int gap, AVPixelFormat fmt, AVRational tb);

	void Init(bool useGPU = false) override;
	AVFrame* Filter(const AVFrame* in) override { return nullptr; };
	AVFrame* Filter(const AVFrame* left, const AVFrame* right) override;

private:
	int m_deviceW;
	int m_deviceH;
	int m_gap;
	AVPixelFormat m_fmt;
	AVRational m_tb;

	AVFilterContext* m_leftSrc = nullptr;
	AVFilterContext* m_rightSrc = nullptr;
};