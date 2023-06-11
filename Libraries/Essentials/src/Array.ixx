export module TR.Essentials.Array;

export import TR.Essentials.Definitions;

import <array>;
import <string>;

namespace TR {

	export template <typename T, size_t length>
	struct CommonArray : public std::array<T, length> {
		static_assert(length > 0, "Length must be greater than 0.");

		using _array = std::array<T, length>;
		using _array::_Elems;
		using _array::array;

		template <typename... Ts>
		CommonArray(T first, Ts... vals) {
			static_assert((std::is_convertible<Ts, T>::value && ...), "Wrong argument type(s) given to CommonArray.");
			static_assert(sizeof...(Ts) == (length - 1), "Wrong number of arguments given to CommonArray.");
			/* 'first' is necessary to allow construction from an implicit template conversion.  */
			_Elems[0] = first;
			size_t i = 0;
			((_Elems[++i] = (T)vals), ...);
		}

		CommonArray(const CommonArray<T, length>& that) : _array(that) {}

		template <typename T_from>
		CommonArray(const CommonArray<T_from, length>& that) {
			operator=(that);
		}

		CommonArray(T val) noexcept {
			Fill(val);
		}

		template <typename T_from, size_t ind = 0>
		CommonArray<T, length>& operator=(const CommonArray<T_from, length>& that) {
			_Elems[ind] = (T)that[ind];
			if constexpr (ind + 1 < length)
				operator=<T_from, ind + 1>(that);
			return *this;
		}

		CommonArray<T, length>& Fill(T val) noexcept {
			for (size_t i = 0; i < length; i++)
				_Elems[i] = val;
			return *this;
		}

		CommonArray<T, length>& Negate() noexcept {
			for (size_t i = 0; i < length; i++)
				_Elems[i] = (T)0 - _Elems[i];
			return *this;
		}

		CommonArray<T, length>& Reciprocate() noexcept {
			for (size_t i = 0; i < length; i++)
				_Elems[i] = (T)1 / _Elems[i];
			return *this;
		}

		_NODISCARD constexpr size_t GetNumBytes() const noexcept {
			return sizeof(_Elems);
		}

		_NODISCARD constexpr size_t GetLength() const noexcept {
			return length;
		}

		_NODISCARD constexpr size_t GetElementSize() const noexcept {
			return sizeof(T);
		}

		_NODISCARD constexpr byte* Bytes() const noexcept {
			return reinterpret_cast<byte*>(this);
		}

		_NODISCARD std::string ToString(std::string prefix = "[", std::string separator = ", ", std::string suffix = "]") const {
			std::string result = prefix;
			for (size_t i = 0; i < length; i++) {
				if constexpr (std::is_convertible<T, std::string>::value)
					result += (std::string)_Elems[i];
				else
					result += std::to_string(_Elems[i]);

				if (i == length - 1)
					result += suffix;
				else
					result += separator;
			}
			return result;
		}

		CommonArray<T, length>& operator+=(const CommonArray<T, length>& that){
			for (size_t i = 0; i < length; i++)
				_Elems[i] += that[i];
			return *this;
		}
		CommonArray<T, length>& operator+=(const T& that) {
			for (size_t i = 0; i < length; i++)
				_Elems[i] += that;
			return *this;
		}
		CommonArray<T, length> operator+(const CommonArray<T, length>& that) {
			CommonArray<T, length> result = *this;
			result += that;
			return result;
		}
		CommonArray<T, length> operator+(const T& that) {
			CommonArray<T, length> result = *this;
			result += that;
			return result;
		}

		CommonArray<T, length>& operator-=(const CommonArray<T, length>& that) {
			for (size_t i = 0; i < length; i++)
				_Elems[i] -= that[i];
			return *this;
		}
		CommonArray<T, length>& operator-=(const T& that) {
			for (size_t i = 0; i < length; i++)
				_Elems[i] -= that;
			return *this;
		}
		CommonArray<T, length> operator-(const CommonArray<T, length>& that) {
			CommonArray<T, length> result = *this;
			result -= that;
			return result;
		}
		CommonArray<T, length> operator-(const T& that) {
			CommonArray<T, length> result = *this;
			result -= that;
			return result;
		}

		CommonArray<T, length>& operator*=(const CommonArray<T, length>& that) {
			for (size_t i = 0; i < length; i++)
				_Elems[i] *= that[i];
			return *this;
		}
		CommonArray<T, length>& operator*=(const T& that) {
			for (size_t i = 0; i < length; i++)
				_Elems[i] *= that;
			return *this;
		}
		CommonArray<T, length> operator*(const CommonArray<T, length>& that) {
			CommonArray<T, length> result = *this;
			result *= that;
			return result;
		}
		CommonArray<T, length> operator*(const T& that) {
			CommonArray<T, length> result = *this;
			result *= that;
			return result;
		}

		CommonArray<T, length>& operator/=(const CommonArray<T, length>& that) {
			for (size_t i = 0; i < length; i++)
				_Elems[i] /= that[i];
			return *this;
		}
		CommonArray<T, length>& operator/=(const T& that) {
			for (size_t i = 0; i < length; i++)
				_Elems[i] /= that;
			return *this;
		}
		CommonArray<T, length> operator/(const CommonArray<T, length>& that) {
			CommonArray<T, length> result = *this;
			result /= that;
			return result;
		}
		CommonArray<T, length> operator/(const T& that) {
			CommonArray<T, length> result = *this;
			result /= that;
			return result;
		}
	};

	export {

		template <typename T, size_t length>
		struct Array : public CommonArray<T, length> {
			using CommonArray<T, length>::CommonArray;
			Array(const CommonArray<T, length>& that) : CommonArray<T, length>(that) {}
		};

		#define TR_ARRAY_ALIAS(x, n)    \
		using n##2 = Array< x , 2>;		\
		using n##3 = Array< x , 3>;		\
		using n##4 = Array< x , 4>;	

		TR_ARRAY_ALIAS(float, Float);
		TR_ARRAY_ALIAS(double, Double);
		TR_ARRAY_ALIAS(int, Int);
		TR_ARRAY_ALIAS(uint, Uint);
		TR_ARRAY_ALIAS(long, Long);
		TR_ARRAY_ALIAS(char, Char);
		TR_ARRAY_ALIAS(byte, Byte);

	}

}