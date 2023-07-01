module;

#include "MediaHeader.h"

export module TR.Media.Scaler;

export import TR.Media.Frame;
export import TR.Media.Exception;

export namespace TR::Media {

	namespace Scaler {

		struct E_FailedContextRetrieval : public _Exception {
			E_FailedContextRetrieval() : _Exception("Failed to get scaler context.") {}
		};
		struct E_FailedScaling : public _MediaException {
			E_FailedScaling(int returnValue) : _MediaException("Failed to scale frame.", returnValue) {}
		};

		struct _Context {
			SwsContext* swsContext = nullptr;

			Int2 srcSize = {}, dstSize = {};
			AVPixelFormat srcFormat = AV_PIX_FMT_NONE, dstFormat = AV_PIX_FMT_NONE;
			int algorithm = SWS_BICUBIC;

			int srcLinesize[8] = {};
			int dstLinesize[8] = {};
		};

		void Init(_Context* scaler, Int2 srcSize, Int2 dstSize, AVPixelFormat srcFormat, AVPixelFormat dstFormat, int algorithm = SWS_BICUBIC);
		bool IsInitialized(_Context* scaler, Int2 srcSize, Int2 dstSize, AVPixelFormat srcFormat, AVPixelFormat dstFormat, int algorithm = SWS_BICUBIC);

		void Scale(_Context* scaler, const Frame::_Context* src, Frame::_Context* dst);
		void Scale(_Context* scaler, const AVFrame* src, Frame::_Context* dst);
		void Scale(_Context* scaler, const Frame::_Context* src, const AVFrame* dst);

		void Release(_Context* scaler);

	}

}