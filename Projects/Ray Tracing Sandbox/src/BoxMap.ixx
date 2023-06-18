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
        } type;
        T_ind refCount = 0; // Probably move to separate buffer later
    };

    struct Container {
        Vector offset = {};
        T_ind node = noInd;
        T_ind sibling = noInd;
        T_ind parent = noInd;
    };

    List<Node> nodes = {};
    List<Container> containers = {};

    auto ObjectHash = [this](const Node& a) -> size_t {
        return std::hash<T_ind>()(a.child) ^ std::hash<decltype(a.size)>()(a.size) ^ std::hash<int>()((int)a.type);
    };

    auto ContainerHash = [this](const Container& a) -> size_t {
        const Node& node = nodes[a.node];
        return ObjectHash(node) ^ std::hash<decltype(a.offset)>()(a.offset);
    };

    auto NodeHash = [this](const Node& a) -> size_t {
        if (a.type == Node::Type::object) {
            return ObjectHash(a);
        }
        else {
            size_t result = std::hash<decltype(a.offset)>()(a.offset) ^ std::hash<int>()((int)a.type);
            for (T_ind cur = a.child; cur != noInd; cur = nodes[cur].sibling)
                result ^= ContainerHash(containers[cur]);
            return result;
        }
    };

    auto NodeEqual = [this](const Node& a, const Node& b) -> bool {
        
    };

    std::unordered_map<Node, T_ind, decltype(NodeHash), decltype(NodeEqual)> nodeMap = {};

    void Init(T_ind size) {
        nodes.Init(size);
        containers.Init(size);

        nodes.New();
        containers.New();

        
    }

    T_ind CreateRoot() {
        T_ind nodeInd = nodes.New();
        Node* node = &nodes[nodeInd];
        node->type = Node::Type::root;
        return nodeInd;
    }

    T_ind CreateObject() {
        T_ind nodeInd = nodes.New();
        Node* node = &nodes[nodeInd];
        node->type = Node::Type::object;
        return nodeInd;
    }

    void Insert(T_ind nodeInd, Vector offset, T_ind rootInd = 0) {

    }

};

/*

Node {
- box
- childIndex
- type (node, rootNode, object)
}
Container {{
- offset
- nodeIndex
- siblingIndex
- parentIndex
}{
- refCount
}}

*/