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
struct E_BadContainerRemoval : public _Exception {
	E_BadContainerRemoval() : _Exception("Container did not belong to the specified node.") {}
};

export template <typename T_val, typename T_ind, size_t numDims>
struct BoxSet {

	static constexpr T_ind noInd = (T_ind)-1;
	static constexpr T_val inf = std::numeric_limits<T_val>::max();
	static constexpr T_val lim = 1.0 / 256.0;
	using Vector = Array<T_val, numDims>;
	using Bool = uint;

	static bool AlmostEquals(Vector a, Vector b, T_val maxError = lim) {
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
		Vector& operator[](bool i) {
			return i ? high : low;
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

	struct Link {
		Array<T_ind, numDims> low = noInd, high = noInd;
		Link() {}
		Link(const Link& that) : low(that.low), high(that.high) {}

		Array<T_ind, numDims>& operator[](bool i) {
			return i ? high : low;
		}

		bool operator==(const Link& that) const {
			return low == that.low && high == that.high;
		}
	};

	struct Node {
		Box box = {};
		union {
			T_ind child; // Used whenever order doesn't matter
			Link childLink = {};
		};
		enum struct Type {
			root = 0,
			helper = 1,
			object = 2
		} type = Type::root;
		
		bool operator==(const Node& that) const {
			return box == that.box && childLink == that.childLink && type == that.type;
		}

		Node() {}
		Node(const Node& that) : box(that.box), childLink(that.childLink), type(that.type) {}
	};

	struct NodeInfo {
		T_ind refCount = 0;
		T_val volume = (T_val)0;
	};

	struct Container {
		Vector offset = {};
		T_ind node = noInd;

		union {
			T_ind sibling; // Used whenever order doesn't matter
			Link siblingLink = {};
		};

		Container() {}
		Container(const Container& that) : offset(that.offset), node(that.node), siblingLink(that.siblingLink) {}
	};

	T_val divisor = (T_val)2;

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

		BoxSet& boxSet;

		bool operator()(const Node& first, const Node& second) const {
			if (first.type != second.type)
				return false;
			if (first.type == Node::Type::helper) {
				T_ind firstInd = first.child;
				T_ind secondInd = second.child;
				while (firstInd != noInd && secondInd != noInd) {
					if (boxSet.containers[firstInd].node != boxSet.containers[secondInd].node)
						return false;
					if (boxSet.containers[firstInd].offset != boxSet.containers[secondInd].offset)
						return false;
					firstInd = boxSet.containers[firstInd].sibling;
					secondInd = boxSet.containers[secondInd].sibling;
				}
				return firstInd == secondInd;
			}
			else {
				return first.box == second.box && first.childLink == second.childLink;
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

	T_ind AddChild(T_ind nodeInd, Vector offset, T_ind parentNodeInd) {
		T_ind childInd = containers.New();
		Container& container = containers[childInd];
		Node& node = nodes[nodeInd];

		container.offset = offset;
		container.node = nodeInd;
		nodeInfo[nodeInd].refCount++;

		Node& parentNode = nodes[parentNodeInd];
		Link& link = parentNode.childLink;

		for (size_t dim = 0; dim < numDims; dim++) {
			auto Less = [this, dim](T_ind a, T_ind b) {
				T_val aVal = containers[a].offset[dim] + nodes[containers[a].node].box.low[dim];
				T_val bVal = containers[b].offset[dim] + nodes[containers[b].node].box.low[dim];
				if (aVal == bVal)
					return a <= b;
				return aVal <= bVal;
			};
			auto Greater = [this, dim](T_ind a, T_ind b) {
				T_val aVal = containers[a].offset[dim] + nodes[containers[a].node].box.high[dim];
				T_val bVal = containers[b].offset[dim] + nodes[containers[b].node].box.high[dim];
				if (aVal == bVal)
					return a <= b;
				return aVal >= bVal;
			};
			for (size_t bound = 0; bound < 2; bound++) {
				if (link[bound][dim] == noInd || ((bound == 0) ? Less(childInd, link[bound][dim]) : Greater(childInd, link[bound][dim]))) {
					container.siblingLink[bound][dim] = link[bound][dim];
					link[bound][dim] = childInd;
				}
				else {
					for (T_ind itInd = link[bound][dim]; itInd != noInd; itInd = containers[itInd].siblingLink[bound][dim]) {
						Link& siblingLink = containers[itInd].siblingLink;
						if (siblingLink[bound][dim] == noInd || ((bound == 0) ? Less(childInd, siblingLink[bound][dim]) : Greater(childInd, siblingLink[bound][dim]))) {
							container.siblingLink[bound][dim] = siblingLink[bound][dim];
							siblingLink[bound][dim] = childInd;
							break;
						}
					}
				}
			}
		}
		return childInd;
	}

	void OffsetNode(T_ind nodeInd, Vector offset) {
		Node& node = nodes[nodeInd];
		for (T_ind childInd = node.child; childInd != noInd; childInd = containers[childInd].sibling)
			containers[childInd].offset += offset;
		node.box.Offset(offset);
	}

	T_ind DuplicateNode(T_ind nodeInd) {
		T_ind newNodeInd = nodes.New();
		Node& newNode = nodes[newNodeInd];
		Node& srcNode = nodes[nodeInd];

		newNode.box = srcNode.box;
		newNode.type = srcNode.type;

		// Can probably be optimized to avoid sorting twice
		for (T_ind childInd = srcNode.child; childInd != noInd; childInd = containers[childInd].sibling) {
			AddChild(containers[childInd].node, containers[childInd].offset, newNodeInd);
		}
		return newNodeInd;
	}

	void DecompressContainer(T_ind ind) {
		Container& container = containers[ind];
		Node& node = nodes[container.node];
		if (nodeInfo[container.node].refCount > 1) {
			nodeInfo[container.node].refCount--;
			container.node = DuplicateNode(container.node);
			nodeInfo[container.node].refCount++;
		}
		else {
			nodeHashMap.erase(node);
		}
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

	void RemoveContainer(T_ind nodeInd, T_ind ind) {
		Node& node = nodes[nodeInd];
		Container& container = containers[ind];

		for (size_t i = 0; i < numDims; i++) {
			for (size_t j = 0; j < 2; j++) {
				if (node.childLink[j][i] == ind) {
					node.childLink[j][i] = container.siblingLink[j][i];
				}
				else {
					for (T_ind childInd = node.childLink[j][i]; childInd != noInd; childInd = containers[childInd].siblingLink[j][i]) {
						if (containers[childInd].siblingLink[j][i] == ind) {
							containers[childInd].siblingLink[j][i] = container.siblingLink[j][i];
							break;
						}
						// Maybe check for invalid call
						if (containers[childInd].siblingLink[j][i] == noInd) {
							throw E_BadContainerRemoval();
						}
					}
				}
			}
		}
		if ((--nodeInfo[container.node].refCount) == 0) {
			RemoveNode(container.node);
		}
		containers.Remove(ind);
	}

	void Insert(T_ind nodeInd, Vector offset, T_val startSum, T_ind rootNodeInd = 0) {
		Node& srcNode = nodes[nodeInd];
		T_val srcSum = srcNode.box.GetSum();

		std::vector<T_ind> path = {};
		T_val curMaxSum = startSum;
		Vector curOffset = {};
		
		for (T_ind curNodeInd = rootNodeInd;;) {
			Node& curNode = nodes[curNodeInd];

			if (curNode.type == Node::Type::root)
				curNode.box.Fit(srcNode.box.GetOffseted(offset - curOffset));

			nodeInfo[curNodeInd].volume += nodeInfo[nodeInd].volume;

			if (srcSum <= curMaxSum / divisor) {
				// Look for helper, if found, create potential duplicate, remove from node, scale box, insert again, if not found, create new helper, scale box and insert
				T_val maxSumRatio = -inf;
				T_ind nextInd = noInd;
				for (T_ind childInd = curNode.child; childInd != noInd; childInd = containers[childInd].sibling) {
					Container& childContainer = containers[childInd];
					Node& child = nodes[childContainer.node];
					if (child.type != Node::Type::helper)
						continue;
					T_val curSum = child.box.GetFit(srcNode.box.GetOffseted(offset - (childContainer.offset + curOffset))).GetSum();
					if (curSum > curMaxSum)
						continue;
					T_val curRatio = (nodeInfo[childContainer.node].volume + nodeInfo[nodeInd].volume) / curSum;
					if (curRatio > maxSumRatio) {
						maxSumRatio = curRatio;
						nextInd = childInd;
					}
				}
				if (nextInd != noInd) {
					DecompressContainer(nextInd);
					Vector oldOffset = containers[nextInd].offset;
					T_ind helperInd = containers[nextInd].node;
					nodeInfo[helperInd].refCount++;
					RemoveContainer(curNodeInd, nextInd);
					nodeInfo[helperInd].refCount--;

					Node& node = nodes[helperInd];
					node.box.Fit(srcNode.box.GetOffseted(offset - (oldOffset + curOffset)));
					Vector newOffset = oldOffset + node.box.low;
					OffsetNode(helperInd, -node.box.low);

					T_ind newContainerInd = AddChild(helperInd, newOffset, curNodeInd);
					path.push_back(newContainerInd);
					curOffset += newOffset;
					curMaxSum /= divisor;
					curNodeInd = helperInd;
				}
				else {
					T_ind helperInd = nodes.New();

					nodes[helperInd].type = Node::Type::helper;
					nodes[helperInd].box = srcNode.box;
					Vector helperOffset = offset - curOffset;
					
					T_ind newContainerInd = AddChild(helperInd, helperOffset, curNodeInd);
					path.push_back(newContainerInd);
					curOffset += helperOffset;
					curMaxSum /= divisor;
					curNodeInd = helperInd;
				}
			}
			else {
				T_ind containerInd = AddChild(nodeInd, offset - curOffset, curNodeInd);
				path.push_back(containerInd);
				break;
			}
		}
		for (T_ind i = 0; i < path.size(); i++) {
			CompressContainer(path[path.size() - (T_ind)1 - i]);
		}
	}

public:

	void InsertObject(Box box, Vector offset, T_ind child, T_val startSum, T_ind rootInd = 0) {
		T_ind nodeInd = CreateObject();
		nodes[nodeInd].box = box;
		nodes[nodeInd].child = child;
		nodeInfo[nodeInd].volume = box.GetSum();
		Insert(nodeInd, offset, startSum, rootInd);
	}

	void InsertRoot(T_ind srcRootNodeInd, Vector offset, T_val startSum, T_ind rootInd = 0) {
		Insert(srcRootNodeInd, offset, startSum, rootInd);
	}

protected:

	bool FindDecompressedPath(Node srcNode, Vector offset, T_ind curNodeInd, std::vector<T_ind>& path) {
		for (T_ind childInd = nodes[curNodeInd].child; childInd != noInd; childInd = containers[childInd].sibling) {
			Container& childContainer = containers[childInd];
			Node& child = nodes[childContainer.node];
			if (child == srcNode && AlmostEquals(childContainer.offset, offset)) {
				DecompressContainer(childInd);
				path.push_back(childInd);
				return true;
			}
			if (child.type != Node::Type::helper)
				continue;
			if (!child.box.GetFit(srcNode.box.GetOffseted(offset - childContainer.offset)).AlmostEquals(child.box)) { // Floating point rounding errors requires "AlmostEquals" :(
				continue;
			}
			DecompressContainer(childInd);
			path.push_back(childInd);
			if (FindDecompressedPath(srcNode, offset - childContainer.offset, childContainer.node, path))
				return true;
			path.pop_back();
		}
		return false;
	}

public:

	void Remove(Node srcNode, Vector offset, T_ind rootNodeInd = 0) {
		std::vector<T_ind> path = {};
		if (!FindDecompressedPath(srcNode, offset, rootNodeInd, path)) {
			throw E_BoxSetBadLocation();
		}
		std::vector<T_ind> nodePath = { rootNodeInd };
		T_val srcVolume = nodeInfo[containers[path.back()].node].volume;
		nodeInfo[rootNodeInd].volume -= srcVolume;
		for (T_ind i = 0; i + 1 < path.size(); i++) {
			nodePath.push_back(containers[path[i]].node);
			nodeInfo[nodePath.back()].volume -= srcVolume;
		}
		nodePath.push_back(containers[path.back()].node);

		// Remove container(s)
		while (path.size() > 0) {
			T_ind containerInd = path.back();
			T_ind parentNodeInd = nodePath[nodePath.size() - 2];

			RemoveContainer(parentNodeInd, containerInd);
			path.pop_back();
			nodePath.pop_back();

			if (nodes[parentNodeInd].child != noInd)
				break;
		}
		// Update bounding boxes
		for (T_ind i = 0; i < nodePath.size(); i++) {
			T_ind nodeInd = nodePath[nodePath.size() - 1 - i];
			Node& node = nodes[nodeInd];
			
			node.box = Box();
			for (T_ind childInd = node.child; childInd != noInd; childInd = containers[childInd].sibling)
				node.box.Fit(nodes[containers[childInd].node].box.GetOffseted(containers[childInd].offset));

			if (node.type != Node::Type::helper)
				continue;

			T_ind containerInd = path[path.size() - 1 - i];
			T_ind parentNodeInd = nodePath[nodePath.size() - 2 - i];
			Container& container = containers[containerInd];
			Vector oldOffset = container.offset;
			nodeInfo[nodeInd].refCount++;
			RemoveContainer(parentNodeInd, containerInd);
			nodeInfo[nodeInd].refCount--;
			
			Vector newOffset = oldOffset + node.box.low;
			OffsetNode(nodeInd, -node.box.low);
			AddChild(nodeInd, newOffset, parentNodeInd);
		}
		for (T_ind i = 0; i < path.size(); i++) {
			T_ind containerInd = path[path.size() - 1 - i];
			CompressContainer(containerInd);
		}
	}

	void RemoveObject(Box box, Vector offset, T_ind child, T_ind rootInd = 0) {
		Node node = {};
		node.box = box;
		node.child = child;
		node.type = Node::Type::object;
		Remove(node, offset, rootInd);
	}

	void RemoveRoot(T_ind srcRootNodeInd, Vector offset, T_ind rootInd = 0) {
		Remove(nodes[srcRootNodeInd], offset, rootInd);
	}

};