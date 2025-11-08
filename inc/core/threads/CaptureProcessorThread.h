#pragma once
#include <wx/thread.h>
class CaptureProcessorThread : public wxThread
{
	virtual ExitCode Entry() override;
};

