module;

#include "MediaHeader.h"

export module TR.Media.FileDecoder;

export import TR.Media.Exception;
export import TR.Media.Scaler;

export namespace TR::Media {

	namespace FileDecoder {

		struct E_FailedFileOpen : public _MediaException {
			E_FailedFileOpen(int returnValue) : _MediaException("Failed to open file.", returnValue) {}
		};
		struct E_NoVideoStream : public _Exception {
			E_NoVideoStream() : _Exception("No video stream found.") {}
		};
		struct E_NoDecoder : public _Exception {
			E_NoDecoder() : _Exception("Failed to find decoder.") {}
		};
		struct E_FailedCodecContextAllocation : public _Exception {
			E_FailedCodecContextAllocation() : _Exception("Failed to allocate codec context.") {}
		};
		struct E_FailedCodecContextCreation : public _MediaException {
			E_FailedCodecContextCreation(int returnValue) : _MediaException("Failed to create codec context.", returnValue) {}
		};
		struct E_FailedCodecContextOpening : public _MediaException {
			E_FailedCodecContextOpening(int returnValue) : _MediaException("Failed to open codec context.", returnValue) {}
		};
		struct E_FailedFrameReading : public _MediaException {
			E_FailedFrameReading(int returnValue) : _MediaException("Failed to read frame.", returnValue) {}
		};
		struct E_FailedPacketSend : public _MediaException {
			E_FailedPacketSend(int returnValue) : _MediaException("Failed to send packet to codec.", returnValue) {}
		};
		struct E_FailedFrameReceival : public _MediaException {
			E_FailedFrameReceival(int returnValue) : _MediaException("Failed to receive frame from codec.", returnValue) {}
		};

		struct _Context {
			const AVCodec* codec = nullptr;
			AVCodecContext* codecContext = nullptr;
			AVFormatContext* formatContext = nullptr;
			uint streamIndex = -1;

			AVFrame* avFrame = nullptr;
			AVPacket* avPacket = nullptr;

			Scaler::_Context scaler = {};
		};

		void Init(_Context* fileDecoder, std::string filePath);

		bool DecodeFrame(_Context* fileDecoder, Frame::_Context* dstFrame, bool allocateFrame = false, int scalingAlgorithm = SWS_BICUBIC);

	}

}