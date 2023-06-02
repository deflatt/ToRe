export module TR.Essentials.Definitions;

export import <type_traits>;

export namespace TR {

	using uchar = unsigned char;
	using byte = uchar;
	using uint = unsigned int;
	using ushort = unsigned short;

	template <typename T_from, typename T_to>
	struct IsDynamicallyConvertible {
		static constexpr bool value = !std::is_convertible<T_from, T_to>::value && std::is_convertible<T_to, T_from>::value &&
			std::is_pointer<T_from>::value && std::is_pointer<T_to>::value &&
			std::has_virtual_destructor<std::remove_pointer<T_from>::type>::value;
	};

}