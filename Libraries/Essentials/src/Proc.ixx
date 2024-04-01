export module TR.Essentials.Proc;

import <functional>;
import <unordered_set>;

export namespace TR {

	namespace Essentials {

		template <typename... Args>
		struct Proc : std::function<void(Args...)> {

			using _function = std::function<void(Args...)>;
			using _function::function;

			/* Might implement some virtual type conversion thing */

		};

		template <typename... Args>
		struct ProcSet : public Proc<Args...> {

			void Insert(Proc<Args...>* proc) {
				procs.insert(proc);
			}

			void Remove(Proc<Args...>* proc) {
				procs.erase(proc);
			}

			bool Contains(Proc<Args...>* proc) {
				return procs.count(proc);
			}

			ProcSet() : Proc<Args...>(
				[this](Args... args) {
					for (Proc<Args...>* proc : procs)
						(*proc)(args...);
				}
			) {}

		protected:

			std::unordered_set<Proc<Args...>*> procs = {};

		};

	}
	using namespace Essentials;

}