module;

#include "MediaHeader.h"

module TR.Media.Encoder;

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

		void Encode(_Context* encoder, const AVFrame* frame, Procedure<const byte*, int> packetHandler)
		{
			int ret;

			ret = avcodec_send_frame(encoder->codecContext, frame);
			if (ret < 0) {
				throw E_FailedFrameSend(ret);
			}

			while (ret >= 0) {
				ret = avcodec_receive_packet(encoder->codecContext, encoder->avPacket);
				if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
					encoder->frameIndex++;
					break;
				}
				if (ret < 0) {
					throw E_FailedPacketReceival(ret);
				}
				packetHandler(encoder->avPacket->data, encoder->avPacket->size);
				av_packet_unref(encoder->avPacket);
			}
		}

		void EncodeFrame(_Context* encoder, const Frame::_Context* frame, Procedure<const byte*, int> packetHandler)
		{
			Scaler::Scale(&encoder->scaler, frame, encoder->avFrame);
			encoder->avFrame->pts = encoder->frameIndex;
			Encode(encoder, encoder->avFrame, packetHandler);
		}

		void Flush(_Context* encoder, Procedure<const byte*, int> packetHandler)
		{
			Encode(encoder, nullptr, packetHandler);
		}

	}

}