export module TR.Essentials.IndexTree;

import <limits>;
export import <vector>;
import <unordered_map>;
import <string>;
import <iostream>;
export import TR.Essentials.Exception;
export import TR.Essentials.Vec;
import TR.Essentials.ElementHeap;

export namespace TR {

	namespace Essentials {

		struct _ReferencedRemovalException : public _Exception {
			_ReferencedRemovalException() : _Exception("Attempted removal of referenced node.") {}
		};

		template <size_t k, std::signed_integral Index = int>
		struct IndexTree {

			static constexpr size_t k = k;
			static constexpr Index voidInd = (Index)0;

			using Location = std::vector<size_t>;

			struct Node {
				
				Vec<Index, k> inds = {};

				Node(Index fillInd = voidInd) {
					for (size_t i = 0; i < k; i++)
						inds[i] = fillInd;
				}
				Node(const Node& that) : inds(that.inds) {}

				bool operator==(const Node& that) const {
					return inds == that.inds;
				}

			};
			struct NodeHash {
				size_t operator()(const Node& node) const {
					size_t result = 0;
					for (size_t i = 0; i < k; i++)
						result ^= std::hash<Index>()(node.inds[i]);
					return result;
				}
			};

			ElementHeap<Node, Index> nodes = {};
			std::unordered_map<Node, Index, NodeHash> nodeMap = {};
			std::vector<size_t> refs = {};

			void Init() {
				nodes.Clear();
				NewNode();

				nodeMap.clear();
				nodeMap.insert({ Node(voidInd), voidInd });
			}

			Index NewNode() {
				Index newInd = -nodes.New();
				nodes[-newInd] = Node();
				return newInd;
			}

			void AddRef(Index ind) {
				if (ind >= voidInd)
					return;
				refs.resize(nodes.Size());
				refs[-ind]++;
			}

			void RemoveRef(Index ind) {
				if (ind >= voidInd)
					return;
				refs[-ind]--;
				if (refs[-ind] == 0)
					RemoveNode(ind);
			}

			void RemoveNode(Index ind) {
				if (ind >= voidInd)
					return;
				if (refs[-ind] != 0)
					throw _ReferencedRemovalException();

				Node& node = nodes[-ind];
				for (size_t i = 0; i < k; i++)
					RemoveRef(nodes[-ind].inds[i]);
				nodes.Remove(-ind);
			}

			Index CopyNode(Index ind) {
				Index newInd = -nodes.New();
				nodes[-newInd] = nodes[-ind];
				for (size_t i = 0; i < k; i++)
					AddRef(nodes[-ind].inds[i]);
				return newInd;
			}

			void Set(Index ind, const Location& loc, Index newInd, size_t locInd = 0) {
				size_t indAtLoc = loc[locInd];
				auto ChildInd = [this, ind, indAtLoc]() -> Index& {
					return nodes[-ind].inds[indAtLoc];
				};

				if (locInd + 1 == loc.size()) {
					RemoveRef(ChildInd());
					ChildInd() = newInd;
					AddRef(ChildInd());
				}
				else {
					if (ChildInd() >= voidInd) {
						ChildInd() = NewNode();
						AddRef(ChildInd());
					}
					else {
						if (refs[-ChildInd()] > 1) {
							RemoveRef(ChildInd());
							ChildInd() = CopyNode(ChildInd());
							AddRef(ChildInd());
						}
						else {
							nodeMap.erase(nodes[-ChildInd()]);
						}
					}
					Set(ChildInd(), loc, newInd, locInd + 1);

					if (nodeMap.contains(nodes[-ChildInd()])) {
						RemoveRef(ChildInd());
						ChildInd() = nodeMap.at(nodes[-ChildInd()]);
						AddRef(ChildInd());
					}
					else {
						nodeMap.insert({ nodes[-ChildInd()], ChildInd() });
					}
				}
			}
			
			Index Get(Index ind, const Location& loc, size_t locInd = 0) {
				if (locInd + 1 == loc.size()) {
					return nodes[-ind].inds[loc[locInd]];
				}
				else {
					return Get(nodes[-ind].inds[loc[locInd]], loc, locInd + 1);
				}
			}

			void Print(Index ind, size_t depth = 0) {
				std::string prefix(depth * 2, ' ');
				if (ind >= voidInd) {
					std::cout << prefix << ind << std::endl;
					return;
				}
				std::cout << prefix << "At " << (-ind) << std::endl;
				for (size_t i = 0; i < k; i++) {
					std::cout << prefix << i << ": ";
					if (nodes[-ind].inds[i] >= voidInd)
						std::cout << nodes[-ind].inds[i] << std::endl;
					else {
						std::cout << std::endl;
						Print(nodes[-ind].inds[i], depth + 1);
					}
				}
			}


		};

	}
	using namespace Essentials;

}