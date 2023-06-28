module;

#include "MediaHeader.h"

export module TR.Media.Frame;

export import TR.Essentials.Array;
export import <vector>;

export namespace TR::Media {

	namespace Frame {

		struct _Context {
			byte* data = {};
			Int2 size = {};
			AVPixelFormat format = AV_PIX_FMT_NONE;
			int bytes_pp;
		};

		void Allocate(_Context* frame, Int2 size, AVPixelFormat format);

		void Release(_Context* context);

	}

}