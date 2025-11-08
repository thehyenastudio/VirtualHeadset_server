#include <core/threads/CaptureProcessorThread.h>

wxThread::ExitCode CaptureProcessorThread::Entry()
{
    while (!TestDestroy()) {
        // Capture processing logic goes here
        // For demonstration, we'll just yield to avoid busy waiting
		wxThread::Yield();
    }
    return 0;
}
