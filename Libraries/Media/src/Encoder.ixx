module;

#include "MediaHeader.h"

export module Encoder;

export import TR.Media.Exception;
export import TR.Media.Scaler;
export import TR.Essentials.Procedure;
export import <vector>;

export namespace TR::Media {

	namespace Encoder {

		struct E_FailedFrameBufferRetrieval : public _MediaException {
			E_FailedFrameBufferRetrieval(int returnValue) : _MediaException("Failed to get frame buffer.", returnValue) {}
		};
		struct E_NoEncoder : public _Exception {
			E_NoEncoder() : _Exception("Failed to find encoder.") {}
		};
		struct E_FailedCodecContextAllocation : public _Exception {
			E_FailedCodecContextAllocation() : _Exception("Failed to allocate codec context.") {}
		};
		struct E_FailedCodecContextOpening : public _MediaException {
			E_FailedCodecContextOpening(int returnValue) : _MediaException("Failed to open codec context.", returnValue) {}
		};

		struct _Context {
			const AVCodec* codec = nullptr;
			AVCodecContext* codecContext = nullptr;

			AVPacket* avPacket = nullptr;
			AVFrame* avFrame = nullptr;
			int frameIndex;

			Scaler::_Context scaler = {};
		};
		
		struct Option {
			std::string name = {};
			std::string val = {};
		};

		void Init(_Context* encoder, AVCodecID codecID, AVPixelFormat dstFormat, Int2 dstSize, int framerate, int bitrate, int gopSize = 10, int maxBframes = 2, std::vector<Option> opts = {});

		void EncodeFrame(_Context* encoder, const Frame::_Context* frame, Procedure<const byte*, int> packetHandler);

	}

}