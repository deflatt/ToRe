export module BoxMap;

export import TR.Essentials.Exception;
export import TR.Essentials.Array;

export import <vector>;
export import <limits>;

using namespace TR;

import <iostream>;

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
    using Position = Array<T_val, numDims>;
    using Bool = uint;

    template <typename T>
    struct List {
        std::vector<T> elements = {};
        T_ind nextElement = 1;
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
    };

    struct Box {
        Position low = Position(inf), high = Position(-inf);
        T_val GetSum() {
            T_val sum = (T_val)0;
            for (size_t i = 0; i < numDims; i++)
                //sum += high[i] - low[i];
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

    using Location = std::vector<T_val>;

    // Indices implicitly point to containers unless otherwise specified

    struct Node {
        Box box = {};
        T_ind child = noInd;
        enum struct Type {
            root = (int)(T_ind)0,
            node = (int)(T_ind)1,
            object = (int)(T_ind)2
        } type;
    };

    struct Container {
        Position offset = {};
        T_ind node = noInd;
        T_ind sibling = noInd;
        T_ind parent = noInd;
        T_ind refCount = 0; // Probably move to separate buffer later
    };

    List<Node> nodes = {};
    List<Container> containers = {};

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