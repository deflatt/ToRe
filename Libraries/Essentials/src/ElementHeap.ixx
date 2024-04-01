export module TR.Essentials.ElementHeap;

import <vector>;

export namespace TR {

	namespace Essentials {

		template <typename Type, typename Index = size_t>
		struct ElementHeap {

		//protected:

			std::vector<Type> elements = {};
			std::vector<Index> removed = {};

		public:

			void Reserve(size_t size) {
				elements.reserve(size);
				removed.reserve(size);
			}

			Index New() {
				if (removed.size()) {
					Index newInd = removed.back();
					removed.pop_back();
					return newInd;
				}
				else {
					elements.push_back(Type());
					return (Index)elements.size() - (Index)1;
				}
			}

			void Remove(Index ind) {
				removed.push_back(ind);
			}

			void Clear() {
				elements.clear();
				removed.clear();
			}

			Index Size() const {
				return (Index)elements.size();
			}

			Type& At(Index ind) {
				return elements[(size_t)ind];
			}
			const Type& At(Index ind) const {
				return elements[(size_t)ind];
			}
			Type& operator[](Index ind) {
				return elements[(size_t)ind];
			}
			const Type& operator[](Index ind) const {
				return elements[(size_t)ind];
			}

		};

	}
	using namespace Essentials;

}