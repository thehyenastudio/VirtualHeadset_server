#include "utils/FFmpegUtils.h"

GPU_ENGINE CheckGPU_Filters()
{
	return GPU_ENGINE::NONE;
}

GPU_ENGINE CheckGPU_Encoder()
{
	const AVCodec* codec = avcodec_find_encoder_by_name("h264_nvenc");
	if (codec)
	{
		return GPU_ENGINE::CUDA;
	}
	codec = avcodec_find_encoder_by_name("hevc_nvenc");
	if (codec)
	{
		return GPU_ENGINE::CUDA;
	}
	return GPU_ENGINE::NONE;
}