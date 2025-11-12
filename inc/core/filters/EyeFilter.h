#pragma once
#include "FilterBase.h"

struct EyeParams
{
	int targetW, targetH;
	int cropW, cropH, cropX, cropY;
	float k1, k2;
};

class EyeFilter : public FilterBase
{
public:
	EyeFilter();

	void Init() override;
	AVFrame* Filter(const AVFrame* in) override;

private:
	int m_srcW;
	int m_srcH;
	AVPixelFormat m_srcFmt;
	AVRational m_tb;
	EyeParams m_params;
};
