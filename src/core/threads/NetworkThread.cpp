#include "core/threads/NetworkThread.h"

NetworkThread::NetworkThread(Queue<PacketPtr>* queue, wxString ip, std::uint16_t port) : wxThread(wxTHREAD_JOINABLE)
{
	m_queue = queue;
	m_ip = ip;
	m_port = port;
}

NetworkThread::~NetworkThread()
{
	if (m_fmtCtx)
	{
		av_write_trailer(m_fmtCtx);
		if (m_fmtCtx->pb) avio_close(m_fmtCtx->pb);
		avformat_free_context(m_fmtCtx);
		m_fmtCtx = nullptr;
		m_stream = nullptr;
	}
}

wxThread::ExitCode NetworkThread::Entry()
{
	wxLog::SetThreadActiveTarget(wxLog::GetActiveTarget());
	SetThreadDescription(GetCurrentThread(), L"ProcessorThread");

	while (!TestDestroy())
	{
		PacketPtr packet;
		if (m_queue->WaitAndPop(packet, this))
		{
			if (packet)
			{
				AVPacket* pkt = packet.get();
				if (!pkt || !m_stream || !m_fmtCtx)
				{
					continue;
				}
				pkt->stream_index = m_stream->index;

				int ret = av_interleaved_write_frame(m_fmtCtx, pkt);
				if (ret < 0)
				{
					wxLogError("RTP write failed (%d) pts=%lld dts=%lld", ret,
							   (long long) pkt->pts, (long long) pkt->dts);
				}
			}
		}
		else
		{
			break;
		}
	}

	return (wxThread::ExitCode) 0;
}

void NetworkThread::InitMuxer(AVCodecContext* encCtx)
{
	std::string url = "rtp://" + std::string(m_ip.mb_str()) + ":" + std::to_string(m_port);
	// Allocate RTP output context
	int ret = avformat_alloc_output_context2(&m_fmtCtx, nullptr, "rtp", url.c_str());
	if (ret < 0 || !m_fmtCtx)
	{
		wxLogError("RTP muxer: avformat_alloc_output_context2 failed (%d)", ret);
	}

	// Add stream using encoder parameters
	m_stream = avformat_new_stream(m_fmtCtx, nullptr);
	if (!m_stream)
	{
		wxLogError("RTP muxer: avformat_new_stream failed");
	}

	ret = avcodec_parameters_from_context(m_stream->codecpar, encCtx);
	if (ret < 0)
	{
		wxLogError("RTP muxer: avcodec_parameters_from_context failed (%d)", ret);
	}

	// Preserve time base for correct timestamps
	m_stream->time_base = encCtx->time_base;

	// Open UDP output
	ret = avio_open(&m_fmtCtx->pb, url.c_str(), AVIO_FLAG_WRITE);
	if (ret < 0)
	{
		wxLogError("RTP muxer: avio_open failed (%d)", ret);
	}

	// Write RTP header
	ret = avformat_write_header(m_fmtCtx, nullptr);
	if (ret < 0)
	{
		wxLogError("RTP muxer: avformat_write_header failed (%d)", ret);
	}

	// Optional: log SDP for debugging
	char sdp[2048] = { 0 };
	av_sdp_create(&m_fmtCtx, 1, sdp, sizeof(sdp));
	wxLogMessage("RTP SDP:\n%s", sdp);
}
