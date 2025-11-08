#pragma once
#include "FrameBGRA.h"
#include <wx/log.h>

class CaptureBase
{
public:
    virtual ~CaptureBase() = default;

    virtual bool Init() = 0;
    virtual FrameBGRA CaptureFrame() = 0;
};

