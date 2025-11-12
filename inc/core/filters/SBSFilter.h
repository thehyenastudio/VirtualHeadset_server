#pragma once
#include "FilterBase.h"

class SBSFilter : public FilterBase
{
public:
	SBSFilter();

	void Init() override;
	AVFrame* Filter(const AVFrame* in) override;

private:
	int m_deviceW, m_deviceH, m_gap;
	AVPixelFormat m_fmt;
};