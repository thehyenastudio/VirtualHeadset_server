#pragma once
#include <wx/log.h>
#include <wx/thread.h>

#include <windows.h>

extern "C" {
#include <libavformat/avformat.h>
}

#include "core/Queue.h"
#include "utils/FFmpegUtils.h"

class NetworkThread : public wxThread
{
public:
	NetworkThread(Queue<PacketPtr>* queue, wxString ip, std::uint16_t port);
	~NetworkThread();
	virtual ExitCode Entry() override;

	void InitMuxer(AVCodecContext* encCtx);

private:
	Queue<PacketPtr>* m_queue = nullptr;

	AVFormatContext* m_fmtCtx = nullptr;
	AVStream* m_stream = nullptr;

	wxString m_ip;
	std::uint16_t m_port = 0;
};