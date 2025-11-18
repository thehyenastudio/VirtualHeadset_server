#pragma once
#include <memory>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/frame.h>
}

enum class GPU_ENGINE
{
	NONE,
	CUDA
};

GPU_ENGINE CheckGPU_Filters();
GPU_ENGINE CheckGPU_Encoder();

struct AVFrameDeleter
{
	void operator()(AVFrame* f) const
	{
		if (f) av_frame_free(&f);
	}
};

struct AVPacketDeleter
{
	void operator()(AVPacket* p) const
	{
		if (p) av_packet_free(&p);
	}
};

using FramePtr = std::unique_ptr<AVFrame, AVFrameDeleter>;
using PacketPtr = std::unique_ptr<AVPacket, AVPacketDeleter>;