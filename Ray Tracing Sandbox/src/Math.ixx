export module Math;

export import TR.Essentials.Array;

using namespace TR;

export template <typename T, size_t length>
void Rotate(Array<T, length>& v, Array<T, length - 1> angles)
{
	for (size_t i = 0; i < length - 1; i++) {
		T c = cos(angles[i]);
		T s = sin(angles[i]);
		T x = v[0] * c - v[i + 1] * s;
		T y = v[i + 1] * c + v[0] * s;
		v[0] = x;
		v[i + 1] = y;
	}
}