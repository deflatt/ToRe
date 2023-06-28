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

	static bool AlmostEquals(Vector a, Vector b, T_val maxError = 1.0f / 256.0f) {
		for (size_t i = 0; i < numDims; i++) {
			T_val error = abs(a[i] - b[i]);
			if (error > maxError)
				return false;
		}
		return true;
	}

	struct Box {
		Vector low = Vector(inf), high = Vector(-inf);
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
		void Offset(Vector offset) {
			for (size_t i = 0; i < numDims; i++) {
				low[i] += offset[i];
				high[i] += offset[i];
			}
		}
		Box GetFit(Box that) {
			Box result = *this;
			result.Fit(that);
			return result;
		}
		Box GetOffseted(Vector offset) {
			Box result = *this;
			result.Offset(offset);
			return result;
		}
		bool operator==(const Box& that) const {
			return low == that.low && high == that.high;
		}
		bool AlmostEquals(const Box& that, T_val maxError = 1.0 / 256.0) const {
			return BoxSet::AlmostEquals(low, that.low) && BoxSet::AlmostEquals(high, that.high);
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

	struct Node {
		Box box = {};
		T_ind child = noInd;
		enum struct Type {
			root = 0,
			helper = 1,
			object = 2
		} type = Type::root;
		
		bool operator==(const Node& that) const {
			return box == that.box && child == that.child && type == that.type;
		}
	};

	struct NodeInfo {
		T_ind refCount = 0;
	};

	struct Container {
		Vector offset = {};
		T_ind node = noInd;
		T_ind sibling = noInd;
	};

	List<Node> nodes = {};
	std::vector<NodeInfo> nodeInfo = {};
	List<Container> containers = {};

	struct NodeHash {
		BoxSet& boxSet;

		size_t operator()(const Node& node) const {
			if (node.type == Node::Type::helper) {
				size_t result = 0;
				for (T_ind childInd = node.child; childInd != noInd; childInd = boxSet.containers[childInd].sibling) {
					Container& childContainer = boxSet.containers[childInd];
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

		BoxSet& boxSet;

		bool operator()(const Node& first, const Node& second) const {
			if (first.type != second.type)
				return false;
			if (first.type == Node::Type::helper) {
				std::unordered_set<OffsetNode, OffsetNode::Hash> firstSet, secondSet;
				for (T_ind childInd = first.child; childInd != noInd; childInd = boxSet.containers[childInd].sibling)
					firstSet.insert({ boxSet.containers[childInd].offset, boxSet.containers[childInd].node });
				for (T_ind childInd = second.child; childInd != noInd; childInd = boxSet.containers[childInd].sibling)
					secondSet.insert({ boxSet.containers[childInd].offset, boxSet.containers[childInd].node });
				return firstSet == secondSet;
			}
			else {
				return first.box == second.box && first.child == second.child;
			}
		}
	};

	using NodeHashMap = std::unordered_map<Node, T_ind, NodeHash, NodeEqualTo>;
	NodeHashMap nodeHashMap = NodeHashMap(0, NodeHash{ *this }, NodeEqualTo{ *this });

	void Init(T_ind size) {
		nodes.Init(size);
		nodeInfo.resize(size);
		containers.Init(size);

		containers[containers.New()].node = CreateRoot();
		nodeInfo[0].refCount++;
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

protected:

	void RemoveNode(T_ind nodeInd) {
		Node& node = nodes[nodeInd];
		if (node.type != Node::Type::object) {
			for (T_ind childInd = node.child; childInd != noInd;) {
				Container& childContainer = containers[childInd];
				T_ind siblingInd = childContainer.sibling;

				if ((--nodeInfo[childContainer.node].refCount) == 0) {
					nodeHashMap.erase(nodes[childContainer.node]);
					RemoveNode(childContainer.node);
				}
				containers.Remove(childInd);
				childInd = siblingInd;
			}
		}
		nodes.Remove(nodeInd);
		nodeInfo[nodeInd] = NodeInfo();
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
			nodeInfo[containers[childInd].node].refCount++;
		}
		return newNodeInd;
	}

	void FixHelperOffset(T_ind ind) {
		Container& container = containers[ind];
		Node& node = nodes[container.node];
		container.offset += node.box.low;
		for (T_ind childInd = node.child; childInd != noInd; childInd = containers[childInd].sibling)
			containers[childInd].offset -= node.box.low;
		node.box.Offset(-node.box.low);
	}

	void CompressContainer(T_ind ind) {
		Container& container = containers[ind];
		Node& node = nodes[container.node];
		
		if (node.type == Node::Type::root)
			return;
		if (nodeHashMap.count(node)) {
			T_ind hashInd = nodeHashMap.at(node);
			if (hashInd != container.node) {
				if ((--nodeInfo[container.node].refCount) == 0) // Should always be true (?)
					RemoveNode(container.node);
				container.node = hashInd;
				nodeInfo[container.node].refCount++;
			}
		}
		else {
			nodeHashMap.insert({ node, container.node });
		}
	}

	void Insert(T_ind nodeInd, Vector offset, T_ind rootNodeInd = 0) {
		Node& srcNode = nodes[nodeInd];

		std::vector<T_ind> path = {};

		T_val curMaxSum = startSum;
		for (T_ind curNodeInd = rootNodeInd;;) {
			Node& curNode = nodes[curNodeInd];
			
			curNode.box.Fit(srcNode.box.GetOffseted(offset));
			if (curNode.type == Node::Type::helper) {
				offset -= curNode.box.low;
				FixHelperOffset(path.back());
			}
			if (srcNode.box.GetSum() <= curMaxSum / divisor) {
				T_val minSum = inf;
				T_ind minSumInd = noInd;
				for (T_ind childInd = curNode.child; childInd != noInd; childInd = containers[childInd].sibling) {
					Container& childContainer = containers[childInd];
					Node& child = nodes[childContainer.node];
					if (child.type != Node::Type::helper)
						continue;
					T_val curSum = child.box.GetFit(srcNode.box.GetOffseted(offset - childContainer.offset)).GetSum();
					if (curSum > curMaxSum)
						continue;
					if (curSum < minSum) {
						minSum = curSum;
						minSumInd = childInd;
					}
				}
				if (minSumInd != noInd) {
					Container& childContainer = containers[minSumInd];
					Node& child = nodes[childContainer.node];
					if (nodeInfo[childContainer.node].refCount > 1) {
						nodeInfo[childContainer.node].refCount--;
						childContainer.node = DuplicateHelper(childContainer.node);
						nodeInfo[childContainer.node].refCount++;
					}
					else {
						nodeHashMap.erase(child);
					}
					path.push_back(minSumInd);
					curNodeInd = childContainer.node;
					offset -= childContainer.offset;
					curMaxSum /= divisor;
					continue;
				}
				T_ind containerInd = AddChild(curNodeInd);
				Container& container = containers[containerInd];
				container.node = nodes.New();
				nodeInfo[container.node].refCount++;
				nodes[container.node].type = Node::Type::helper;

				path.push_back(containerInd);
				curNodeInd = container.node;
				curMaxSum /= divisor;
			}
			else {
				T_ind containerInd = AddChild(curNodeInd);
				Container& container = containers[containerInd];
				container.offset = offset;
				container.node = nodeInd;
				nodeInfo[nodeInd].refCount++;
				path.push_back(containerInd);
				break;
			}
		}
		for (T_ind i = 0; i < path.size(); i++) {
			CompressContainer(path[path.size() - (T_ind)1 - i]);
		}
	}

public:

	void InsertObject(Box box, Vector offset, T_ind child, T_ind rootInd = 0) {
		T_ind nodeInd = CreateObject();
		nodes[nodeInd].box = box;
		nodes[nodeInd].child = child;
		Insert(nodeInd, offset, rootInd);
	}

	void InsertRoot(T_ind srcRootNodeInd, Vector offset, T_ind rootInd = 0) {
		Insert(srcRootNodeInd, offset, rootInd);
	}

protected:

	bool FindPath(Node srcNode, Vector offset, T_ind curNodeInd, std::vector<T_ind>& path) {
		for (T_ind childInd = nodes[curNodeInd].child; childInd != noInd; childInd = containers[childInd].sibling) {
			Container& childContainer = containers[childInd];
			Node& child = nodes[childContainer.node];
			if (child == srcNode && AlmostEquals(childContainer.offset, offset)) {
				path.push_back(childInd);
				return true;
			}
			if (child.type != Node::Type::helper)
				continue;
			if (!child.box.GetFit(srcNode.box.GetOffseted(offset - childContainer.offset)).AlmostEquals(child.box)) { // Floating point rounding errors requires "AlmostEquals" :(
				Box b1 = srcNode.box.GetOffseted(offset - childContainer.offset);
				Box b2 = child.box;
				continue;
			}
			path.push_back(childInd);
			if (FindPath(srcNode, offset - childContainer.offset, childContainer.node, path))
				return true;
			path.pop_back();
		}
		return false;
	}

public:

	void Remove(Node srcNode, Vector offset, T_ind rootNodeInd = 0) {
		std::vector<T_ind> path = {};
		if (!FindPath(srcNode, offset, rootNodeInd, path)) {
			throw E_BoxSetBadLocation();
		}
		// Decompress path
		for (T_ind i = 0; i + 1 < path.size(); i++) {
			Container& container = containers[path[i]];
			Node& node = nodes[container.node];
			if (nodeInfo[container.node].refCount > 1) {
				nodeInfo[container.node].refCount--;
				container.node = DuplicateHelper(container.node);
				nodeInfo[container.node].refCount++;
				T_ind newChildInd = nodes[container.node].child;
				for (T_ind oldChildInd = node.child; oldChildInd != noInd; oldChildInd = containers[oldChildInd].sibling) {
					if (oldChildInd == path[i + 1]) {
						path[i + 1] = newChildInd;
						break;
					}
					newChildInd = containers[newChildInd].sibling;
				}
			}
			else {
				nodeHashMap.erase(node);
			}
		}
		// Remove node(s)
		T_ind pathEnd;
		for (pathEnd = 0; pathEnd < path.size(); pathEnd++) {
			T_ind containerInd = path[path.size() - 1 - pathEnd];
			Container& container = containers[containerInd];
			Node& node = nodes[container.node];

			Container& parentContainer = containers[path[path.size() - 2 - pathEnd]];
			T_ind parentNodeInd;
			if (pathEnd + 1 == path.size())
				parentNodeInd = rootNodeInd;
			else
				parentNodeInd = containers[path[path.size() - 2 - pathEnd]].node;
			Node& parent = nodes[parentNodeInd];

			if (parent.child == containerInd) {
				parent.child = container.sibling;
			}
			else {
				for (T_ind childInd = parent.child; childInd != noInd; childInd = containers[childInd].sibling) {
					Container& childContainer = containers[childInd];
					if (childContainer.sibling == containerInd) {
						childContainer.sibling = container.sibling;
						break;
					}
					if (childContainer.sibling == noInd) {
						throw E_BoxSetBadLocation(); // TODO: Fix exception
					}
				}
			}
			if ((--nodeInfo[container.node].refCount) == 0) {
				RemoveNode(container.node);
			}
			containers.Remove(containerInd);
			if (parent.child != noInd)
				break;
		}
		// Update bounding boxes
		for (T_ind i = pathEnd + 1; i < path.size() + 1; i++) {
			T_ind containerInd;
			T_ind nodeInd;
			if (i == path.size())
				nodeInd = rootNodeInd;
			else {
				containerInd = path[path.size() - 1 - i];
				nodeInd = containers[containerInd].node;
			}
			Node& node = nodes[nodeInd];
			
			node.box = Box();
			for (T_ind childInd = node.child; childInd != noInd; childInd = containers[childInd].sibling)
				node.box.Fit(nodes[containers[childInd].node].box.GetOffseted(containers[childInd].offset));

			if (node.type == Node::Type::helper)
				FixHelperOffset(containerInd);
		}
		for (T_ind i = pathEnd + 1; i < path.size(); i++) {
			T_ind containerInd = path[path.size() - 1 - i];
			CompressContainer(containerInd);
		}
	}

	void RemoveObject(Box box, Vector offset, T_ind child, T_ind rootInd = 0) {
		Remove({ box, child, Node::Type::object }, offset, rootInd);
	}

	void RemoveRoot(T_ind srcRootNodeInd, Vector offset, T_ind rootInd = 0) {
		Remove(nodes[srcRootNodeInd], offset, rootInd);
	}

};