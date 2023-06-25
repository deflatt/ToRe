export module BoxSet;

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

struct E_BoxSetListOverflow : public _Exception {
	E_BoxSetListOverflow() : _Exception("BoxSet list overflow.") {}
};
struct E_BoxSetBadLocation : public _Exception {
	E_BoxSetBadLocation() : _Exception("Invalid BoxSet location.") {}
};
struct E_BoxSetBadRemoval : public _Exception {
	E_BoxSetBadRemoval() : _Exception("Helper nodes cannot be removed.") {}
};
struct E_BoxSetNoReference : public _Exception {
	E_BoxSetNoReference() : _Exception("Cannot remove reference from a non-referenced node.") {}
};

export template <typename T_val, typename T_ind, size_t numDims, T_val startSum, T_val divisor = (T_val)2>
struct BoxSet {

	static constexpr T_ind noInd = (T_ind)-1;
	static constexpr T_val inf = std::numeric_limits<T_val>::max();
	using Vector = Array<T_val, numDims>;
	using Bool = uint;

	struct Box {
		Vector low = Position(inf), high = Position(-inf);
		T_val GetSum() {
			T_val sum = (T_val)0;
			for (size_t i = 0; i < numDims; i++)
				sum = std::max(sum, high[i] - low[i]);
			return sum;
		}
		void Fit(Box that) {
			for (size_t i = 0; i < numDims; i++) {
				if (that.low[i] < low[i])
					low[i] = that.low[i];
				if (that.high[i] > high[i])
					high[i] = that.high[i];
			}
		}
		Box GetFit(Box that) {
			Box result = *this;
			result.Fit(that);
			return result;
		}
	};

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
					throw E_BoxSetListOverflow();
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

	// Indices implicitly point to containers unless otherwise specified

	// Should probably bring back bounding boxes in nodes, although 'low' will always be 0 in helper nodes

	struct Node {
		Box box = {};
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
	};

	List<Node> nodes = {};
	List<Container> containers = {};

	struct NodeHash {
		BoxSet& BoxSet;

		size_t operator()(const Node& node) const {
			if (node.type == Node::Type::helper) {
				size_t result = 0;
				for (T_ind childInd = node.child; childInd != noInd; childInd = BoxSet.containers[childInd].sibling) {
					Container& childContainer = BoxSet.containers[childInd];
					result ^= ArrayHash<T_val, numDims>()(childContainer.offset);
					result ^= std::hash<T_ind>()(childContainer.node);
				}
				return result;
			}
			else {
				return ArrayHash<T_val, numDims>()(node.box.low) ^ ArrayHash<T_val, numDims>()(node.box.high) ^ std::hash<T_ind>()(node.child);
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

		BoxSet& BoxSet;

		bool operator()(const Node& first, const Node& second) const {
			if (first.type != second.type)
				return false;
			if (first.type == Node::Type::helper) {
				std::unordered_set<OffsetNode, OffsetNode::Hash> firstSet, secondSet;
				for (T_ind childInd = first.child; childInd != noInd; childInd = BoxSet.containers[childInd].sibling)
					firstSet.insert({ BoxSet.containers[childInd].offset, BoxSet.containers[childInd].node });
				for (T_ind childInd = second.child; childInd != noInd; childInd = BoxSet.containers[childInd].sibling)
					secondSet.insert({ BoxSet.containers[childInd].offset, BoxSet.containers[childInd].node });
				return firstSet == secondSet;
			}
			else {
				return first.size == second.size && first.child == second.child;
			}
		}
	};

	using NodeHashMap = std::unordered_map<Node, T_ind, NodeHash, NodeEqualTo>;
	NodeHashMap nodeHashMap = NodeHashMap(0, NodeHash{ *this }, NodeEqualTo{ *this });
	//std::vector<Node> hashedNodes = {};

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

	void InsertObject(Vector position, Vector size, T_ind child, T_ind rootInd = 0) {
		T_ind nodeInd = CreateObject();
		nodes[nodeInd].size = size;
		nodes[nodeInd].child = child;
		return Insert(nodeInd, position, rootInd);
	}

protected:

	void RemoveReference(T_ind nodeInd) {
		Node& node = nodes[nodeInd];
		if (node.refCount == 0) {
			throw E_BoxSetNoReference();
		}
		node.refCount--;
		if (node.type != Node::Type::root && node.refCount == 0) {
			if (node.type == Node::Type::helper) {
				for (T_ind childInd = node.child; childInd != noInd; childInd = containers[childInd].sibling)
					RemoveReference(containers[childInd].node);
			}
			nodeHashMap.erase(node);
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

		newNode.box = srcNode.box;
		newNode.type = Node::Type::helper;

		for (T_ind childInd = srcNode.child; childInd != noInd; childInd = containers[childInd].sibling) {
			T_ind newChildInd = AddChild(newNodeInd);
			containers[newChildInd].offset = containers[childInd].offset;
			containers[newChildInd].node = containers[childInd].node;
			nodes[containers[childInd].node].refCount++;
		}
		return newNodeInd;
	}

	void FixHelperOffset(T_ind ind) {
		Container& container = containers[ind];
		Node& node = nodes[container.node];
		for (T_ind childInd = node.child; childInd != noInd; childInd = containers[childInd].sibling)
			containers[childInd].offset -= node.box.low - container.offset;
		container.offset = node.box.low;
		node.box.high -= node.box.low;
		node.box.low = {};
	}

	void Fit(T_ind rootInd, T_ind nodeInd, Vector& offset) {
		Container& rootContainer = containers[rootInd];
		Node& rootNode = nodes[rootContainer.node];
		Node& node = nodes[nodeInd];

		Float3 newLow, newHigh;
		for (size_t i = 0; i < numDims; i++) {
			newLow[i] = std::min(offset[i], (rootNode.child == noInd) ? inf : rootContainer.offset[i]);
			newHigh[i] = std::max(offset[i] + node.size[i], (rootNode.child == noInd) ? -inf : (rootContainer.offset[i] + rootNode.size[i]));
		}
		if (newLow != rootContainer.offset)
			SetNodeOffset(rootInd, newLow);
		rootNode.size = newHigh - newLow;
		offset -= rootContainer.offset;
	}

	// TODO: Add nodes to hashmap

	void CompressContainer(T_ind ind) {
		Container& container = containers[ind];
		Node& node = nodes[container.node];

		if (node.type == Node::Type::root)
			return;
		
		if (nodeHashMap.count(node)) {
			T_ind hashInd = nodeHashMap.at(node);
			if (hashInd != container.node) {
				RemoveReference(container.node);
				container.node = hashInd;
				nodes[container.node].refCount++;
			}
		}
		else {
			nodeHashMap.insert({ node, container.node });
		}
	}

	void Insert(T_ind nodeInd, Vector offset, T_ind rootNodeInd = 0) {
		Node& srcNode = nodes[nodeInd];

		std::vector<T_ind> path = {};

		Vector curOffset = {};
		float curMaxSum = startSum;
		for (T_ind curNodeInd = rootNodeInd;;) {
			Node& curNode = nodes[nodeInd];

			curNode.box.Fit(srcNode.box);
			if (curNode.type == Node::Type::helper) {
				FixHelperOffset(curNodeInd);
			}

			if (srcNode.box.GetSum() <= curMaxSum / divisor) {
				bool deeper = false;
				// TODO: Change to go to the lowest sum
				for (T_ind childInd = curNode.child; childInd != noInd; childInd = containers[childInd].sibling) {
					Container& childContainer = containers[childInd];
					Node& child = nodes[childContainer.node];
					if (child.type != Node::Type::helper)
						continue;
					if (child.box.GetFit(srcNode.box).GetSum() > curMaxSum)
						continue;
					if (child.refCount > 1) {
						RemoveReference(childContainer.node);
						childContainer.node = DuplicateHelper(childContainer.node);
						nodes[childContainer.node].refCount++;
					}
					else {
						nodeHashMap.erase(child);
					}
					path.push_back(childInd);
					curNodeInd = childContainer.node;
					curMaxSum /= divisor;
					curOffset += childContainer.offset;
					deeper = true;
					break;
				}
				if (deeper)
					continue;
				T_ind containerInd = AddChild(curNode);
				Container& container = containers[containerInd];
				container.offset = curOffset;
				container.node = nodes.New();
				nodes[container.node].refCount++;
				nodes[container.node].type = Node::Type::helper;

				path.push_back(containerInd);
				curNodeInd = container.node;
				curMaxSum /= divisor;
				curOffset += container.offset;
			}
			else {

			}
		}

	}

public:

	// Should return the location of the first non-empty parent
	// Should always free when reaching 0 references
	void Remove(Location location) {
		if (nodes[containers[location.back()].node].type == Node::Type::helper) {
			throw E_BoxSetBadRemoval();
		}

		for (T_ind i = 1; i + 1 < location.size(); i++) {
			Container& container = containers[location[i]];
			Node& node = nodes[container.node];
			if (node.refCount > 1) {
				RemoveReference(container.node);
				container.node = DuplicateHelper(container.node);
				nodes[container.node].refCount++;
				T_ind newChildInd = nodes[container.node].child;
				for (T_ind oldChildInd = node.child; oldChildInd != noInd; oldChildInd = containers[oldChildInd].sibling) {
					if (oldChildInd == location[i + 1]) {
						location[i + 1] = newChildInd;
						break;
					}
					newChildInd = containers[newChildInd].sibling;
				}
			}
			else {
				nodeHashMap.erase(node);
			}
		}
		
		while (location.size() > 1) {
			Container& container = containers[location.back()];
			Node& node = nodes[container.node];
			Container& parentContainer = containers[location[location.size() - 2]];
			Node& parent = nodes[parentContainer.node];

			if (parent.child == location.back()) {
				parent.child = container.sibling;
			}
			else {
				for (T_ind childInd = parent.child; childInd != noInd; childInd = containers[childInd].sibling) {
					Container& childContainer = containers[childInd];
					if (childContainer.sibling == noInd) {
						throw E_BoxSetBadLocation();
					}
					if (childContainer.sibling == location.back()) {
						childContainer.sibling = container.sibling;
						break;
					}
				}
			}
			RemoveReference(container.node);
			containers.Remove(location.back());
			location.pop_back();

			if (parent.child != noInd)
				break;
		}
		// Update bounding boxes
		for (T_ind i = 0; i < location.size(); i++) {
			T_ind ind = location[location.size() - 1 - i];
			Container& container = containers[ind];
			Node& node = nodes[container.node];
			if (node.child == noInd)
				continue;

			Vector low(inf), high(-inf);
			for (T_ind childInd = node.child; childInd != noInd; childInd = containers[childInd].sibling) {
				Vector offset = containers[childInd].offset;
				Vector size = nodes[containers[childInd].node].size;
				for (size_t i = 0; i < numDims; i++) {
					low[i] = std::min(low[i], offset[i]);
					high[i] = std::max(high[i], offset[i] + size[i]);
				}
			}
			node.size = high - low;
			SetNodeOffset(ind, container.offset + low);
		}

		CompressLocation(location);
	}

};