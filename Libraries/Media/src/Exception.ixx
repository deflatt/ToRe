export module TR.Media.Exception;

export import TR.Essentials.Exception;

export namespace TR::Media {

	struct _MediaException : public _Exception {
		int returnValue;
		_MediaException(std::string msg = {}, int returnValue = 0) : _Exception(msg), returnValue(returnValue) {}
	};

}