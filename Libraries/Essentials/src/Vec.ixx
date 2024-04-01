export module TR.Essentials.Vec;

export import TR.Essentials.Definitions;
export import <string>;
import <initializer_list>;

export namespace TR {

	namespace Essentials {

		template <typename Type, size_t n>
		struct Vec {

			Type elems[n] = {};

			Vec(Type val = (Type)0) {
				for (size_t i = 0; i < n; i++)
					elems[i] = val;
			}
			Vec(std::initializer_list<Type> list) {
				size_t i = 0;
				for (const Type& elem : list)
					elems[i++] = elem;
			}
			template <typename... Types>
			Vec(Types... inElems) : elems{ (Type)inElems... } {}

			Type& operator[](size_t i) {
				return elems[i];
			}
			const Type& operator[](size_t i) const {
				return elems[i];
			}

		};

		template <typename Type, size_t n>
		Vec<Type, n> operator+(Vec<Type, n> v1, Vec<Type, n> v2) {
			Vec<Type, n> result;
			for (size_t i = 0; i < n; i++)
				result[i] = v1[i] + v2[i];
			return result;
		}
		template <typename Type, size_t n>
		Vec<Type, n> operator-(Vec<Type, n> v1, Vec<Type, n> v2) {
			Vec<Type, n> result;
			for (size_t i = 0; i < n; i++)
				result[i] = v1[i] - v2[i];
			return result;
		}
		template <typename Type, size_t n>
		Vec<Type, n> operator*(Vec<Type, n> v, Type s) {
			Vec<Type, n> result;
			for (size_t i = 0; i < n; i++)
				result[i] = v[i] * s;
			return result;
		}
		template <typename Type, size_t n>
		Vec<Type, n> operator*(Type s, Vec<Type, n> v) {
			return v * s;
		}
		template <typename Type, size_t n>
		Vec<Type, n> operator/(Vec<Type, n> v, Type s) {
			Vec<Type, n> result;
			for (size_t i = 0; i < n; i++)
				result[i] = v[i] / s;
			return result;
		}
		template <typename Type, size_t n>
		Vec<Type, n> operator-(Vec<Type, n> v) {
			return (Type)(-1) * v;
		}

		template <typename Type, size_t n>
		Type operator*(Vec<Type, n> v1, Vec<Type, n> v2) {
			Type result = (Type)0;
			for (size_t i = 0; i < n; i++)
				result += v1[i] * v2[i];
			return result;
		}

		template <typename Type, size_t n>
		bool operator==(Vec<Type, n> v1, Vec<Type, n> v2) {
			for (size_t i = 0; i < n; i++)
				if (v1[i] != v2[i])
					return false;
			return true;
		}

		template <typename Type, size_t n>
		std::string ToString(Vec<Type, n> vec, std::string prefix = "[", std::string separator = ", ", std::string suffix = "]") {
			std::string result = prefix;
			for (size_t i = 0; i < n; i++)
				result += std::to_string(vec[i]) + ((i == n - 1) ? suffix : separator);
			return result;
		}

		using Float2 = Vec<float, 2>;
		using Float3 = Vec<float, 3>;
		using Float4 = Vec<float, 4>;
		
		using Int2 = Vec<int, 2>;
		using Int3 = Vec<int, 3>;
		using Int4 = Vec<int, 4>;

		using Uint2 = Vec<uint, 2>;
		using Uint3 = Vec<uint, 2>;
		using Uint4 = Vec<uint, 4>;

		using Byte2 = Vec<byte, 2>;
		using Byte3 = Vec<byte, 3>;
		using Byte4 = Vec<byte, 4>;


	}
	using namespace Essentials;

}

