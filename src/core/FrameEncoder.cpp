#include "core/FrameEncoder.h"

FrameEncoder::FrameEncoder()
{}

FrameEncoder::~FrameEncoder()
{}

void FrameEncoder::Init()
{
	const AVCodec* codec = nullptr;

	switch (CheckGPU_Encoder())
	{
		case GPU_ENGINE::NONE:
			codec = avcodec_find_encoder(AV_CODEC_ID_H264);
			break;
		case GPU_ENGINE::CUDA:
			codec = avcodec_find_encoder_by_name("h264_nvenc");
			break;
	}

	m_ctx = avcodec_alloc_context3(codec);

	m_ctx->width = 1920;
	m_ctx->height = 1080;
	m_ctx->time_base = { 1, 60 };
	m_ctx->framerate = { 60, 1 };

	if (codec && std::string(codec->name) == "h264_nvenc")
	{
		m_ctx->pix_fmt = AV_PIX_FMT_NV12;
		av_opt_set(m_ctx->priv_data, "preset", "p4", 0);
		av_opt_set(m_ctx->priv_data, "rc", "cbr", 0);
		av_opt_set(m_ctx->priv_data, "tune", "ull", 0);   // ultra low latency
		av_opt_set_int(m_ctx->priv_data, "delay", 0, 0);
		av_opt_set(m_ctx->priv_data, "zerolatency", "1", 0);
		av_opt_set_int(m_ctx->priv_data, "max_b_frames", 0, 0);
		av_opt_set_int(m_ctx->priv_data, "repeat_sps", 1, 0);
	}
	else
	{
		m_ctx->pix_fmt = AV_PIX_FMT_YUV420P;
		av_opt_set(m_ctx->priv_data, "preset", "veryfast", 0);
		av_opt_set(m_ctx->priv_data, "tune", "zerolatency", 0);
		m_ctx->max_b_frames = 0;
		m_ctx->gop_size = 120; // or appropriate GOP for your fps
		av_opt_set(m_ctx->priv_data, "repeat_headers", "1", 0);
	}

	int ret = avcodec_open2(m_ctx, codec, nullptr);
	if (ret < 0)
	{
		wxLogError("avcodec_open2 failed (%d)", ret);
	}

	// Prepare scaler from BGRA to encoder pix_fmt
	m_sws = sws_getContext(
		m_ctx->width, m_ctx->height, AV_PIX_FMT_BGRA,
		m_ctx->width, m_ctx->height, m_ctx->pix_fmt,
		SWS_BILINEAR, nullptr, nullptr, nullptr);

	// Allocate a reusable destination frame with the encoder format
	m_bufferFrame = av_frame_alloc();
	m_bufferFrame->format = m_ctx->pix_fmt;
	m_bufferFrame->width = m_ctx->width;
	m_bufferFrame->height = m_ctx->height;
	av_frame_get_buffer(m_bufferFrame, 32);
	m_pts = 0;
}

void FrameEncoder::ChangeConfig()
{}

PacketPtr FrameEncoder::EncodePacket(FramePtr frame)
{
	if (!m_ctx) return PacketPtr(nullptr);

	const uint8_t* srcData[4] = { frame.get()->data[0], nullptr, nullptr, nullptr };
	int srcLinesize[4] = { frame.get()->linesize[0], 0, 0, 0 };

	sws_scale(m_sws,
			  srcData, srcLinesize, 0, m_ctx->height,
			  m_bufferFrame->data, m_bufferFrame->linesize);

	m_bufferFrame->pts = m_pts++;

	// Send frame to encoder
	int ret = avcodec_send_frame(m_ctx, m_bufferFrame);
	if (ret < 0)
	{
		wxLogError("Error sending frame to encoder: %d", ret);
		return PacketPtr(nullptr);
	}
	// Receive encoded packet
	AVPacket* pkt = av_packet_alloc();
	ret = avcodec_receive_packet(m_ctx, pkt);
	if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
	{
		av_packet_free(&pkt);
		return PacketPtr(nullptr);
	}
	else if (ret < 0)
	{
		wxLogError("Error receiving packet: %d", ret);
		av_packet_free(&pkt);
		return PacketPtr(nullptr);
	}

	wxLogDebug("Encoded pkt pts=%lld dts=%lld size=%d", (long long) pkt->pts, (long long) pkt->dts, pkt->size);

	return PacketPtr(pkt);
}
