module;

#include "MediaHeader.h"

module TR.Media.FileDecoder;

namespace TR::Media {

	namespace FileDecoder {

		void Init(_Context* fileDecoder, std::string filePath)
		{
			int ret;

			fileDecoder->formatContext = avformat_alloc_context();

			ret = avformat_open_input(&fileDecoder->formatContext, filePath.c_str(), nullptr, nullptr);
			if (ret < 0) {
				throw E_FailedFileOpen(ret);
			}

			fileDecoder->streamIndex = -1;
			for (uint i = 0; i < fileDecoder->formatContext->nb_streams; i++) {
				if (fileDecoder->formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
					fileDecoder->streamIndex = i;
					break;
				}
			}
			if (fileDecoder->streamIndex == -1) {
				throw E_NoVideoStream();
			}

			fileDecoder->codec = avcodec_find_decoder(fileDecoder->formatContext->streams[fileDecoder->streamIndex]->codecpar->codec_id);
			if (!fileDecoder->codec) {
				throw E_NoDecoder();
			}

			fileDecoder->codecContext = avcodec_alloc_context3(fileDecoder->codec);
			if (!fileDecoder->codecContext) {
				throw E_FailedCodecContextAllocation();
			}
			ret = avcodec_parameters_to_context(fileDecoder->codecContext, fileDecoder->formatContext->streams[fileDecoder->streamIndex]->codecpar);
			if (ret < 0) {
				throw E_FailedCodecContextCreation(ret);
			}
			ret = avcodec_open2(fileDecoder->codecContext, fileDecoder->codec, nullptr);
			if (ret < 0) {
				throw E_FailedCodecContextOpening(ret);
			}

			fileDecoder->avFrame = av_frame_alloc();
			fileDecoder->avPacket = av_packet_alloc();
		}

		bool DecodeFrame(_Context* fileDecoder, Frame::_Context* dstFrame, bool allocateFrame, int scalingAlgorithm)
		{
			int ret;
			while (true) {
				while (true) {
					ret = av_read_frame(fileDecoder->formatContext, fileDecoder->avPacket);
					if (ret < 0) {
						if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
							return false;
						else
							throw E_FailedFrameReading(ret);
					}
					if (fileDecoder->avPacket->stream_index == fileDecoder->streamIndex)
						break;
					av_packet_unref(fileDecoder->avPacket);
				}
				ret = avcodec_send_packet(fileDecoder->codecContext, fileDecoder->avPacket);
				if (ret < 0) {
					throw E_FailedPacketSend(ret);
				}
				av_packet_unref(fileDecoder->avPacket);

				ret = avcodec_receive_frame(fileDecoder->codecContext, fileDecoder->avFrame);
				if (ret < 0) {
					if (ret == AVERROR(EAGAIN))
						continue;
					else if (ret == AVERROR_EOF)
						return false;
					else
						throw E_FailedFrameReceival(ret);
				}
				break;
			}
			if (allocateFrame) {
				Frame::Allocate(dstFrame, {
					dstFrame->size[0] ? dstFrame->size[0] : fileDecoder->avFrame->width,
					dstFrame->size[1] ? dstFrame->size[1] : fileDecoder->avFrame->height },
					dstFrame->format != AV_PIX_FMT_NONE ? dstFrame->format : (AVPixelFormat)fileDecoder->avFrame->format);
			}
			fileDecoder->scaler.algorithm = scalingAlgorithm;
			Scaler::Scale(&fileDecoder->scaler, fileDecoder->avFrame, dstFrame);
			return true;
		}

		void Release(_Context* fileDecoder)
		{
			if (fileDecoder->codecContext) {
				avcodec_free_context(&fileDecoder->codecContext);
				fileDecoder->codecContext = nullptr;
			}
			if (fileDecoder->formatContext) {
				avformat_close_input(&fileDecoder->formatContext);
				avformat_free_context(fileDecoder->formatContext);
				fileDecoder->formatContext = nullptr;
			}
			if (fileDecoder->avFrame) {
				av_frame_free(&fileDecoder->avFrame);
				fileDecoder->avFrame = nullptr;
			}
			if (fileDecoder->avPacket) {
				av_packet_free(&fileDecoder->avPacket);
				fileDecoder->avPacket = nullptr;
			}
			Scaler::Release(&fileDecoder->scaler);
		}

	}

}