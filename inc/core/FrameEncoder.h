#pragma once
#include <wx/log.h>

#include <cstdint>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/base64.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <libavutil/opt.h>
}

#include "utils/FFmpegUtils.h"

class FrameEncoder
{
public:
	FrameEncoder();
	~FrameEncoder();

	AVCodecContext* GetContext() const { return m_ctx; };

	void Init();
	void ChangeConfig();
	PacketPtr EncodePacket(FramePtr frame);


private:
	AVCodecContext* m_ctx = nullptr;
	SwsContext* m_sws = nullptr;

	AVFrame* m_bufferFrame = nullptr;
	std::uint16_t m_pts = 0;
};