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
	EyeFilter(int srcW, int srcH, AVPixelFormat srcFmt, AVRational tb, const EyeParams& params);

	void Init(bool useGPU = false) override;
	AVFrame* Filter(const AVFrame* in) override;
	AVFrame* Filter(const AVFrame* left, const AVFrame* right) override { return nullptr; };

private:
	int m_srcW;
	int m_srcH;
	AVPixelFormat m_srcFmt;
	AVRational m_tb;
	EyeParams m_params;
};
