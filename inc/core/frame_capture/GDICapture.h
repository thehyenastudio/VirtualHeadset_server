#pragma once
#include "CaptureBase.h"

class GDICapture : public CaptureBase
{
public:
	GDICapture() = default;
	~GDICapture() override = default;

	bool Init() override;
	AVFrame* CaptureFrame() override;
};

