export module BoxMap;

export import TR.Essentials.Exception;
export import TR.Essentials.Array;

export import <vector>;
export import <limits>;
export import <unordered_map>;

using namespace TR;

import <iostream>;

template <typename T, size_t length>
struct std::hash<Array<T, length>> {

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
	E_BoxMapBadRemoval() : _Exception("Only locked containers can be removed.") {}
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

	using Location = std::vector<T_val>;

	// Indices implicitly point to containers unless otherwise specified

	struct Node {
		//Vector offset = {};
		Vector size = {};
		T_ind child = noInd;
		enum struct Type {
			root = 0,
			node = 1,
			object = 2
		} type = Type::root;
		//T_ind refCount = 0; // Probably move to separate buffer later
	};

	struct Container {
		Vector offset = {};
		T_ind node = noInd;
		T_ind sibling = noInd;
		T_ind parent = noInd; // Temporary
	};

	List<Node> nodes = {};
	List<Container> containers = {};

	void Init(T_ind size) {
		nodes.Init(size);
		containers.Init(size);
		
		containers[containers.New()].node = nodes.New();
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
		return location;
	}

	Location InsertObject(Vector position, Vector size, T_ind child, T_ind rootInd = 0) {
		T_ind nodeInd = CreateObject();
		nodes[nodeInd].size = size;
		nodes[nodeInd].child = child;
		return Insert(nodeInd, position, rootInd);
	}

protected:

	T_val GetSum(Vector size) {
		T_val sum = 0.0f;
		for (size_t i = 0; i < 3; i++)
			sum = std::max(size[i], sum);
		return sum;
	}

	T_ind AddChild(T_ind parentInd) {
		Node& parent = nodes[containers[parentInd].node];
		T_ind childInd = containers.New();
		containers[childInd].sibling = parent.child;
		containers[childInd].parent = parentInd;
		parent.child = childInd;
		return childInd;
	}

	void SetNodeOffset(T_ind ind, Vector offset) {
		Container& container = containers[ind];
		Node& node = nodes[container.node];
		for (T_ind childInd = node.child; childInd != noInd; childInd = containers[childInd].sibling)
			containers[childInd].offset -= offset - container.offset;
		container.offset = offset;
	}

	// offset is relative to the origin that the root lies in
	void Insert(T_ind nodeInd, Vector offset, T_val maxChildSum, T_ind rootInd, Location& location) {
		location.push_back(rootInd);

		Container& rootContainer = containers[rootInd];
		Node& rootNode = nodes[rootContainer.node];
		Node& node = nodes[nodeInd];

		{
			Float3 newLow, newHigh;
			for (size_t i = 0; i < 3; i++) {
				newLow[i] = std::min(offset[i], rootContainer.offset[i]);
				newHigh[i] = std::max(offset[i] + node.size[i], rootContainer.offset[i] + rootNode.size[i]);
			}
			if (newLow != rootContainer.offset)
				SetNodeOffset(rootInd, newLow);
			rootNode.size = newHigh - newLow;
			offset -= rootContainer.offset;
		}

		float sum = GetSum(node.size);
		if (sum <= maxChildSum) {
			for (T_ind childInd = rootNode.child; childInd != noInd; childInd = containers[childInd].sibling) {
				Container& childContainer = containers[childInd];
				Node& child = nodes[childContainer.node];
				if (child.type != Node::Type::node)
					continue;

				// Maybe change the responsibility of checking if it fits to the current container, and have it ignore the fit-check if it's a root
				Vector size;
				for (size_t i = 0; i < 3; i++)
					size[i] = std::max(offset[i] + node.size[i], childContainer.offset[i] + child.size[i])
					- std::min(offset[i], childContainer.offset[i]);
				if (GetSum(size) > maxChildSum)
					continue;

				Insert(nodeInd, offset, maxChildSum / divisor, childInd, location);
				return;
			}
			T_ind containerInd = AddChild(rootInd);
			Container& container = containers[containerInd];
			container.offset = offset;
			container.node = nodes.New();
			nodes[container.node].type = Node::Type::node;
			Insert(nodeInd, offset, maxChildSum / divisor, containerInd, location);
			return;
		}
		else {
			T_ind containerInd = AddChild(rootInd);
			Container& container = containers[containerInd];
			container.offset = offset;
			container.node = nodeInd;
			location.push_back(containerInd);
		}
	}

public:

	// Add ability to remove without freeing
	void Remove(Location location) {
		T_ind containerInd = location.back();
		Container& container = containers[containerInd];
		Node& node = nodes[container.node];
		
		Container& parentContainer = containers[container.parent];
		Node& parent = nodes[parentContainer.node];

		location.pop_back();
		if (parent.child == containerInd) {
			parent.child = container.sibling;
			if (parent.child == noInd) {
				Remove(location);
			}
		}
		else {
			for (T_ind childInd = parent.child; childInd != noInd; childInd = containers[childInd].sibling) {
				Container& childContainer = containers[childInd];
				if (childContainer.sibling == noInd) {
					throw E_BoxMapBadLocation();
				}
				if (childContainer.sibling == containerInd) {
					childContainer.sibling = container.sibling;
					break;
				}
			}
		}
		// Update bounding boxes!
		nodes.Remove(container.node);
		containers.Remove(containerInd);
	}

};