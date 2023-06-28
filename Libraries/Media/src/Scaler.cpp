module;

#include "MediaHeader.h"

module TR.Media.Scaler;

namespace TR::Media {

	namespace Scaler {

		void Init(_Context* scaler, Int2 srcSize, Int2 dstSize, AVPixelFormat srcFormat, AVPixelFormat dstFormat, int algorithm)
		{
			scaler->srcSize = srcSize;
			scaler->dstSize = dstSize;
			scaler->srcFormat = srcFormat;
			scaler->dstFormat = dstFormat;
			scaler->algorithm = algorithm;

			scaler->swsContext = sws_getContext(srcSize[0], srcSize[1], srcFormat, dstSize[0], dstSize[1], dstFormat, algorithm, nullptr, nullptr, nullptr);
			if (!scaler->swsContext) {
				throw E_FailedContextRetrieval();
			}
			scaler->srcLinesize[0] = srcSize[0] * av_get_bits_per_pixel(av_pix_fmt_desc_get(srcFormat)) / 8;
			scaler->dstLinesize[0] = dstSize[0] * av_get_bits_per_pixel(av_pix_fmt_desc_get(dstFormat)) / 8;
		}

		bool IsInitialized(_Context* scaler, Int2 srcSize, Int2 dstSize, AVPixelFormat srcFormat, AVPixelFormat dstFormat, int algorithm)
		{
			return (scaler->swsContext &&
				scaler->srcSize == srcSize && scaler->dstSize == dstSize &&
				scaler->srcFormat == srcFormat && scaler->dstFormat == dstFormat &&
				scaler->algorithm == algorithm);
		}

		void Scale(_Context* scaler, const Frame::_Context* src, Frame::_Context* dst)
		{
			if (!IsInitialized(scaler, src->size, dst->size, src->format, dst->format, scaler->algorithm)) {
				Release(scaler);
				Init(scaler, src->size, dst->size, src->format, dst->format, scaler->algorithm);
			}
			int ret = sws_scale(scaler->swsContext, &src->data, scaler->srcLinesize, 0, scaler->srcSize[0], &dst->data, scaler->dstLinesize);
			if (ret < 0) {
				throw E_FailedScaling(ret);
			}
		}

		void TR::Media::Scaler::Scale(_Context* scaler, const AVFrame* src, Frame::_Context* dst)
		{
			if (!IsInitialized(scaler, { src->width, src->height }, dst->size, (AVPixelFormat)src->format, dst->format, scaler->algorithm)) {
				Release(scaler);
				Init(scaler, { src->width, src->height }, dst->size, (AVPixelFormat)src->format, dst->format, scaler->algorithm);
			}
			int ret = sws_scale(scaler->swsContext, src->data, src->linesize, 0, scaler->srcSize[1], &dst->data, scaler->dstLinesize);
			if (ret < 0) {
				throw E_FailedScaling(ret);
			}
		}

		void Release(_Context* scaler)
		{
			if (scaler->swsContext) {
				sws_freeContext(scaler->swsContext);
				scaler->swsContext = nullptr;
			}
		}

	}

}