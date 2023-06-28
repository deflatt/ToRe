module;

#include "MediaHeader.h"

module TR.Media.Frame;

namespace TR::Media {

	namespace Frame {

		void Allocate(_Context* frame, Int2 size, AVPixelFormat format)
		{
			frame->size = size;
			frame->format = format;
			frame->bytes_pp = av_get_bits_per_pixel(av_pix_fmt_desc_get(format)) / 8;
			frame->data = new byte[size[0] * size[1] * frame->bytes_pp];
		}

		void Release(_Context* context)
		{
			if (context->data) {
				delete context->data;
				context->data = nullptr;
			}
		}

	}

}