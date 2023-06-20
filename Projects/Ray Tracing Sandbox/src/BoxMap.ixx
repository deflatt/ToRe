export module BoxMap;

export import TR.Essentials.Exception;
export import TR.Essentials.Array;

export import <vector>;
export import <limits>;
export import <unordered_set>;
export import <unordered_map>;

using namespace TR;

import <iostream>;

export template <typename T, size_t length>
struct ArrayHash {

	static constexpr size_t numBits = sizeof(size_t) * (size_t)8;

	size_t operator()(const Array<T, length>& a) const {
		size_t result = 0;
		for (size_t i = 0; i < length; i++) {
			size_t curHash = std::hash<T>()(a[i]);
			result ^= (curHash << (i % numBits)) | (curHash >> (numBits - (i % numBits)));
		}
		return result;
	}

};

struct E_BoxMapListOverflow : public _Exception {
	E_BoxMapListOverflow() : _Exception("BoxMap list overflow.") {}
};
struct E_BoxMapBadLocation : public _Exception {
	E_BoxMapBadLocation() : _Exception("Invalid BoxMap location.") {}
};
struct E_BoxMapBadRemoval : public _Exception {
	E_BoxMapBadRemoval() : _Exception("Helper nodes cannot be removed.") {}
};
struct E_BoxMapNoReference : public _Exception {
	E_BoxMapNoReference() : _Exception("Cannot remove reference from a non-referenced node.") {}
};

export template <typename T_val, typename T_ind, size_t numDims, T_val startSum, T_val divisor = (T_val)2>
struct BoxMap {

	static constexpr T_ind noInd = (T_ind)-1;
	static constexpr T_val inf = std::numeric_limits<T_val>::max();
	using Vector = Array<T_val, numDims>;
	using Bool = uint;

	template <typename T>
	struct List {
		std::vector<T> elements = {};
		T_ind nextElement = 0;
		std::vector<T_ind> removed = {};
		T_ind numRemoved = 0;

		void Init(T_ind size) {
			elements.resize(size, T());
			removed.resize(size, noInd);
		}

		T_ind New() {
			if (numRemoved) {
				return removed[--numRemoved];
			}
			else {
				if (nextElement == elements.size()) {
					throw E_BoxMapListOverflow();
				}
				return nextElement++;
			}
		}

		void Remove(T_ind ind) {
			elements[ind] = T();
			removed[numRemoved++] = ind;
		}

		T& operator[](T_ind ind) {
			return elements[ind];
		}
	};

	using Location = std::vector<T_ind>;

	// Indices implicitly point to containers unless otherwise specified

	struct Node {
		//Vector offset = {};
		Vector size = {};
		T_ind child = noInd;
		enum struct Type {
			root = 0,
			helper = 1,
			object = 2
		} type = Type::root;
		T_ind refCount = 0; // Probably move to separate buffer later
	};

	struct Container {
		Vector offset = {};
		T_ind node = noInd;
		T_ind sibling = noInd;
		//T_ind parent = noInd; // Temporary
	};

	List<Node> nodes = {};
	List<Container> containers = {};

	struct NodeHash {
		BoxMap& boxMap;

		size_t operator()(const Node& node) const {
			if (node.type == Node::Type::helper) {
				size_t result = 0;
				for (T_ind childInd = node.child; childInd != noInd; childInd = boxMap.containers[childInd].sibling) {
					Container& childContainer = boxMap.containers[childInd];
					result ^= ArrayHash<T_val, numDims>()(childContainer.offset);
					result ^= std::hash<T_ind>()(childContainer.node);
				}
				return result;
			}
			else {
				return ArrayHash<T_val, numDims>()(node.size) ^ std::hash<T_ind>()(node.child);
			}
		}
	};

	struct NodeEqualTo {

		struct OffsetNode {
			Vector offset = {};
			T_ind node = noInd;

			struct Hash {
				size_t operator()(const OffsetNode& node) const {
					return ArrayHash<T_val, numDims>()(node.offset) ^ std::hash<T_ind>()(node.node);
				}
			};
			
			bool operator==(const OffsetNode& that) const {
				return offset == that.offset && node == that.node;
			}
		};

		BoxMap& boxMap;

		//void GetSet(const Node& node, std::unordered_set<OffsetNode, OffsetNode::Hash>& set) {
		//	for (T_ind childInd = node.child; childInd != noInd; childInd = boxMap.containers[childInd].sibling)
		//		set.insert({ boxMap.containers[childInd].offset, boxMap.containers[childInd].node });
		//}

		bool operator()(const Node& first, const Node& second) const {
			if (first.type != second.type)
				return false;
			if (first.type == Node::Type::helper) {
				std::unordered_set<OffsetNode, OffsetNode::Hash> firstSet, secondSet;
				//GetSet(first, firstSet);
				//GetSet(second, secondSet);
				for (T_ind childInd = first.child; childInd != noInd; childInd = boxMap.containers[childInd].sibling)
					firstSet.insert({ boxMap.containers[childInd].offset, boxMap.containers[childInd].node });
				for (T_ind childInd = second.child; childInd != noInd; childInd = boxMap.containers[childInd].sibling)
					secondSet.insert({ boxMap.containers[childInd].offset, boxMap.containers[childInd].node });
				return firstSet == secondSet;
			}
			else {
				return first.size == second.size && first.child == second.child;
			}
		}
	};

	std::unordered_map<Node, T_ind, NodeHash, NodeEqualTo> nodeHashMap
		= std::unordered_map<Node, T_ind, NodeHash, NodeEqualTo>(0, NodeHash{ *this }, NodeEqualTo{ *this });

	void Init(T_ind size) {
		nodes.Init(size);
		containers.Init(size);
		
		containers[containers.New()].node = nodes.New();
		nodes[0].refCount++;
	}

	T_ind CreateRoot() {
		T_ind nodeInd = nodes.New();
		Node& node = nodes[nodeInd];
		node.type = Node::Type::root;
		return nodeInd;
	}

	T_ind CreateObject() {
		T_ind nodeInd = nodes.New();
		Node& node = nodes[nodeInd];
		node.type = Node::Type::object;
		return nodeInd;
	}

	Location Insert(T_ind nodeInd, Vector offset, T_ind rootInd = 0) {
		Location location = {};
		Insert(nodeInd, offset, startSum, rootInd, location);
		UpdateLocation(location);
		return location;
	}

	Location InsertObject(Vector position, Vector size, T_ind child, T_ind rootInd = 0) {
		T_ind nodeInd = CreateObject();
		nodes[nodeInd].size = size;
		nodes[nodeInd].child = child;
		return Insert(nodeInd, position, rootInd);
	}

protected:

	struct Box {
		Vector position;
		Vector size;
	};
	
	Box GetFit(Box parent, Box child) {
		Vector low, high;
		for (size_t i = 0; i < numDims; i++) {
			low[i] = std::min(parent.position[i], child.position[i]);
			high[i] = std::max(parent.position[i] + parent.size[i], child.position[i] + child.size[i]);
		}
		return { low, high - low };
	}

	T_val GetSum(Vector size) {
		T_val sum = 0.0f;
		for (size_t i = 0; i < numDims; i++)
			sum = std::max(size[i], sum);
		return sum;
	}

	void RemoveReference(T_ind nodeInd) {
		Node& node = nodes[nodeInd];
		if (node.refCount == 0) {
			throw E_BoxMapNoReference();
		}
		node.refCount--;
		if (node.type == Node::Type::helper && nodes[nodeInd].refCount == 0) {
			for (T_ind childInd = node.child; childInd != noInd; childInd = containers[childInd].sibling)
				RemoveReference(containers[childInd].node);
			nodes.Remove(nodeInd);
		}
	}

	T_ind AddChild(T_ind parentNodeInd) {
		Node& parent = nodes[parentNodeInd];
		T_ind childInd = containers.New();
		containers[childInd].sibling = parent.child;
		parent.child = childInd;
		return childInd;
	}
	
	T_ind DuplicateHelper(T_ind nodeInd) {
		T_ind newNodeInd = nodes.New();
		Node& newNode = nodes[newNodeInd];
		Node& srcNode = nodes[nodeInd];

		newNode.size = srcNode.size;
		newNode.type = Node::Type::helper;
		
		for (T_ind childInd = srcNode.child; childInd != noInd; childInd = containers[childInd].sibling) {
			T_ind newChildInd = AddChild(newNodeInd);
			containers[newChildInd].offset = containers[childInd].offset;
			containers[newChildInd].node = containers[childInd].node;
			nodes[containers[childInd].node].refCount++;
		}
		return newNodeInd;
	}

	void SetNodeOffset(T_ind ind, Vector offset) {
		Container& container = containers[ind];
		Node& node = nodes[container.node];
		for (T_ind childInd = node.child; childInd != noInd; childInd = containers[childInd].sibling)
			containers[childInd].offset -= offset - container.offset;
		container.offset = offset;
	}

	// TODO: Add nodes to hashmap

	void UpdateLocation(Location location) {
		for (T_ind i = 0; i < location.size(); i++) {
			Container& container = containers[(T_ind)location.size() - i - 1];
			Node& node = nodes[container.node];
			
			// Fix box
			if (node.type == Node::Type::helper) {
				// Maybe change to only check the child node
				Vector low(inf), high(-inf);
				for (T_ind childInd = node.child; childInd != noInd; childInd = containers[childInd].sibling) {
					Container childContainer = containers[childInd];
					Node& childNode = nodes[childContainer.node];
					for (size_t i = 0; i < numDims; i++) {
						low[i] = std::min(low[i], childContainer.offset[i]);
						high[i] = std::max(high[i], childContainer.offset[i] + childNode.size[i]);
					}
				}
				for (T_ind childInd = node.child; childInd != noInd; childInd = containers[childInd].sibling)
					containers[childInd].offset -= low - container.offset;
				container.offset = low;
				node.size = high - low;
			}

			// Compress
			if (nodeHashMap.count(node)) {
				RemoveReference(container.node);
				container.node = nodeHashMap.at(node);
				nodes[container.node].refCount++;
			}
		}
	}

	// offset is relative to the origin that the root lies in
	void Insert(T_ind nodeInd, Vector offset, T_val maxChildSum, T_ind rootInd, Location& location) {
		location.push_back(rootInd);

		Container& rootContainer = containers[rootInd];
		Node& rootNode = nodes[rootContainer.node];
		Node& node = nodes[nodeInd];

		offset -= rootContainer.offset;

		Box nodeBox = { offset, node.size };

		float sum = GetSum(node.size);
		if (sum <= maxChildSum) {
			// TODO: Maybe greedily find the best match instead of inserting into the first acceptable one
			for (T_ind childInd = rootNode.child; childInd != noInd; childInd = containers[childInd].sibling) {
				Container& childContainer = containers[childInd];
				Node& child = nodes[childContainer.node];
				if (child.type != Node::Type::helper)
					continue;
				
				Box childBox = { childContainer.offset, child.size };
				Box fitBox = GetFit(childBox, nodeBox);
				if (GetSum(fitBox.size) > maxChildSum)
					continue;
				if (child.refCount > 1) {
					RemoveReference(childContainer.node);
					childContainer.node = DuplicateHelper(childContainer.node);
					nodes[childContainer.node].refCount++;
				}
				Insert(nodeInd, offset, maxChildSum / divisor, childInd, location);
				return;
			}
			T_ind containerInd = AddChild(rootContainer.node);
			Container& container = containers[containerInd];
			//container.offset = offset;
			container.node = nodes.New();
			nodes[container.node].refCount++;
			nodes[container.node].type = Node::Type::helper;
			Insert(nodeInd, offset, maxChildSum / divisor, containerInd, location);
		}
		else {
			T_ind containerInd = AddChild(rootContainer.node);
			Container& container = containers[containerInd];
			container.offset = offset;
			container.node = nodeInd;
			node.refCount++;
			location.push_back(containerInd);
		}
	}

public:

	// Should return the location of the first non-empty parent
	// Add ability to remove without freeing
	void Remove(Location location) {
		T_ind containerInd = location.back();
		Container& container = containers[containerInd];
		Node& node = nodes[container.node];
		
		if (node.type == Node::Type::helper) {
			throw E_BoxMapBadRemoval();
		}

		
	}

};