module;

#include "MediaHeader.h"

module Encoder;

namespace TR::Media {

	namespace Encoder {

		void Init(_Context* encoder, AVCodecID codecID, AVPixelFormat dstFormat, Int2 dstSize, int framerate, int bitrate, int gopSize, int maxBframes, std::vector<Option> opts)
		{
			int ret;

			encoder->avPacket = av_packet_alloc();
			encoder->avFrame = av_frame_alloc();
			
			encoder->avFrame->width = dstSize[0];
			encoder->avFrame->height= dstSize[1];
			encoder->avFrame->format = dstFormat;

			ret = av_frame_get_buffer(encoder->avFrame, 0);
			if (ret < 0) {
				throw E_FailedFrameBufferRetrieval(ret);
			}

			encoder->codec = avcodec_find_encoder(codecID);
			if (!encoder->codec) {
				throw E_NoEncoder();
			}

			encoder->codecContext = avcodec_alloc_context3(encoder->codec);
			if (!encoder->codecContext) {
				throw E_FailedCodecContextAllocation();
			}

			encoder->codecContext->width = encoder->avFrame->width;
			encoder->codecContext->height = encoder->avFrame->height;
			encoder->codecContext->framerate = { framerate, 1 };
			encoder->codecContext->time_base = { 1, framerate };
			encoder->codecContext->bit_rate = bitrate;
			encoder->codecContext->pix_fmt = (AVPixelFormat)encoder->avFrame->format;
			encoder->codecContext->gop_size = gopSize;
			encoder->codecContext->max_b_frames = maxBframes;

			for (Option opt : opts) {
				av_opt_set(encoder->codecContext->priv_data, opt.name.c_str(), opt.val.c_str(), 0);
			}

			ret = avcodec_open2(encoder->codecContext, encoder->codec, nullptr);
			if (ret < 0) {
				throw E_FailedCodecContextOpening(ret);
			}

			encoder->frameIndex = 0;
		}

		void EncodeFrame(_Context* encoder, const Frame::_Context* frame, Procedure<const byte*, int> packetHandler)
		{
			
		}

	}

}