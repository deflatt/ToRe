export module TR.Essentials.AABBTree;

export import TR.Essentials.IndexTree;

export namespace TR {

	namespace Essentials {

		template <typename Type, Type a, Type b>
		constexpr Type Pow() {
			if constexpr (b == (Type)0)
				return (Type)1;
			else
				return a * Pow<Type, a, b - (Type)1>();
		}


		template <std::signed_integral Index, std::floating_point Bound, size_t dim = 3, size_t k = 2>
		struct AABBTree : public IndexTree<Pow<size_t, k, dim>(), Index> {

			struct AABB {
				Vec<Bound, dim> lower = Vec<Bound, dim>(std::numeric_limits<Bound>::infinity());
				Vec<Bound, dim> higher = Vec<Bound, dim>(-std::numeric_limits<Bound>::infinity());
			};

		protected:

			using _IndexTree = IndexTree<Pow<size_t, k, dim>(), Index>;
			using _IndexTree::Set;

			std::vector<AABB> bounds = {};

		public:

			using Location = std::vector<Vec<size_t, dim>>;

			void FixBounds(Index ind, std::vector<size_t>& itLoc, size_t locInd = 0) {
				auto node = _IndexTree::nodes[-ind];
				if (locInd + 1 != itLoc.size()) {
					Index childInd = node.inds[itLoc[locInd]];
					FixBounds(childInd, itLoc, locInd + 1);
				}
				AABB newBounds = {};
				for (size_t i = 0; i < Pow<size_t, k, dim>(); i++) {
					if (node.inds[i] == _IndexTree::voidInd)
						continue;
					size_t il = i;
					AABB cur = {};
					for (size_t j = 0; j < dim; j++) {
						cur.lower[j] = (Bound)(il % k) / (Bound)k;
						il /= k;
					}
					if (node.inds[i] > _IndexTree::voidInd) {
						cur.higher = cur.lower + Vec<Bound, dim>((Bound)1) / (Bound)k;
					}
					else {
						AABB childBound = bounds[-node.inds[i]];
						cur.higher = cur.lower + childBound.higher / (Bound)k;
						cur.lower = cur.lower + childBound.lower / (Bound)k;
					}
					for (size_t i = 0; i < dim; i++) {
						if (cur.lower[i] < newBounds.lower[i])
							newBounds.lower[i] = cur.lower[i];
						if (cur.higher[i] > newBounds.higher[i])
							newBounds.higher[i] = cur.higher[i];
					}
				}
				if (!(-ind < bounds.size()))
					bounds.resize(-ind + 1);
				bounds[-ind] = newBounds;
			}

			void SetNode(Index ind, const Location& loc, Index newInd) {
				std::vector<size_t> itLoc = {};
				for (size_t i = 0; i < loc.size(); i++) {
					itLoc.push_back(0);
					size_t mul = 1;
					for (size_t d = 0; d < dim; d++) {
						itLoc.back() += loc[i][d] * mul;
						mul *= k;
					}
				}
				Set(ind, itLoc, newInd);
				FixBounds(ind, itLoc);
			}

			AABB GetBounds(Index ind) {
				return bounds[-ind];
			}

		};

	}
	using namespace Essentials;

}