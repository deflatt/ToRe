export module TR.Essentials.Procedure;

export import TR.Essentials.Definitions;
import <functional>;
import <unordered_map>;

export namespace TR {

	template <typename... T_args>
	struct Procedure : public std::function<void(T_args...)> {

		using _function = std::function<void(T_args...)>;
		using _function::function;

	protected:

		// Necessary because some weird IIFE bug has been introduced :(
		template <typename T_from, typename T_to>
		static constexpr bool IsConvertible(T_from& from) {
			if constexpr (std::is_convertible<T_from, T_to>::value)
				return true;
			if (from == nullptr)
				return true;
			from = reinterpret_cast<T_from>(dynamic_cast<T_to>(from));
			return (from != nullptr);
		}

	public:

		template <typename... T_to>
		Procedure(Procedure<T_to...> to) :
			_function(
				[to](T_args... args) {
					bool convertible = (IsConvertible<T_args, T_to>(args) && ...);
					if (!convertible)
						return;
					to(((T_to)args)...);
				}
			) {
			static_assert(sizeof...(T_args) == sizeof...(T_to), "Can't convert between argument lengths.");
			static_assert(((std::is_convertible<T_args, T_to>::value || IsDynamicallyConvertible<T_args, T_to>::value) && ...), "Can't convert arguments.");
		}
	};

	template <typename... T_args>
	struct ProcedureSet : public Procedure<T_args...> {

		void Add(Procedure<T_args...> proc) {
			map.insert({ 0, proc });
		}

		template <typename... T_from>
		void Insert(Procedure<T_from...>* proc) {
			map.insert({ (size_t)proc, (Procedure<T_args...>)(*proc) });
		}

		template <typename... T_from>
		void Remove(Procedure<T_from...>* proc) {
			map.erase((size_t)proc);
		}

		template <typename... T_from>
		bool Contains(Procedure<T_from...>* proc) {
			return map.count((size_t)proc);
		}

		ProcedureSet() : Procedure<T_args...>(
			[this](T_args... args) {
				for (auto& proc : map)
					proc.second(args...);
			}) {}

	protected:

		std::unordered_multimap<size_t, Procedure<T_args...>> map = {};

	};

}