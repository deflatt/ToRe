#include <functional>
#include <iostream>
#include <Windows.h>
#include <map>
#include <tuple>
#include <set>
#include <limits>
#undef min
#undef max

import TR.Essentials.Array;
using namespace TR;

template <typename T_val, typename T_ind, size_t numDims, T_val startSum, T_val divisor = (T_val)2>
struct BoxMap {

    static constexpr T_ind noInd = (T_ind)-1;
    static constexpr T_ind delInd = (T_ind)-2;
    static constexpr T_val inf = std::numeric_limits<T_val>::max();
    using Position = Array<T_val, numDims>;
    using Bool = uint;

    struct Box {
        Position low = Position(inf), high = Position(-inf);
        T_val GetSum() {
            T_val sum = (T_val)0;
            for (size_t i = 0; i < numDims; i++)
                sum += high[i] - low[i];
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

    struct Node {
        Box box = {};
        T_ind childContainer = noInd;
    };

    struct Container {
        T_ind node = noInd;
        T_ind siblingContainer = noInd;
        Bool locked = false;
    };

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
                    std::cout << "BoxMap list overflow." << std::endl;
                    exit(0);
                }
                return nextElement++;
            }
        }

        void Remove(T_ind ind) {
            elements[ind] = T();
            removed[numRemoved++] = ind;
        }
    };

protected:

    List<Node> nodes = {};
    List<Container> containers = {};

    T_ind AddChildContainer(T_ind parentNodeInd) {
        Node* parent = &nodes.elements[parentNodeInd];
        T_ind containerInd = containers.New();
        Container* container = &containers.elements[containerInd];
        container->siblingContainer = parent->childContainer;
        parent->childContainer = containerInd;
        return containerInd;
    }

    T_ind AddChild(T_ind parentNodeInd) {
        T_ind containerInd = AddChildContainer(parentNodeInd);
        Container* container = &containers.elements[containerInd];
        container->node = nodes.New();
        return containerInd;
    }

    void Insert(Node insertNode, Location& location, T_val maxSum) {
        Container* parentContainer = &containers.elements[location.back()];
        Node* parent = &nodes.elements[parentContainer->node];
        
        parent->box.Fit(insertNode.box);

        // If the new node's sum is less than maxSum, it should be placed in a new node if it doesn't fit in an existing one


        // TODO: Make the positions relative before implementing compression

        if (insertNode.box.GetSum() <= maxSum / divisor) {
            for (T_ind childInd = parent->childContainer; childInd != noInd; childInd = containers.elements[childInd].siblingContainer) {
                Container* childContainer = &containers.elements[childInd];
                if (childContainer->locked)
                    continue;
                Node* child = &nodes.elements[childContainer->node];

                T_val sum = child->box.GetFit(insertNode.box).GetSum();
                if (sum <= maxSum) {
                    location.push_back(childInd);
                    Insert(insertNode, location, maxSum / divisor);
                    return;
                }
            }
            T_ind containerInd = AddChild(parentContainer->node);
            location.push_back(containerInd);
            Insert(insertNode, location, maxSum / divisor);
        }
        else {
            T_ind containerInd = AddChild(parentContainer->node);
            location.push_back(containerInd);
            Container* container = &containers.elements[containerInd];
            container->locked = true;
            nodes.elements[container->node] = insertNode;
        }

        //Node* parent = &nodes.elements[location.back()];
        // Maybe check if it fits first, should only be necessary for the root
        

        // Spara bara ner själva container indexen, den bör vara unik i ett sibling set
        //T_ind index = 0;
        //for (T_ind childInd = parent->child; childInd != noInd; childInd = nodes.elements[childInd].sibling) {
        //    Node* child = &nodes.elements[childInd];
        //
        //    T_val sum = child->box.GetSum(box);
        //    if (sum <= maxSum) {
        //        location.push_back();
        //        return Insert(box, childInd, maxSum / divisor);
        //    }
        //    index++;
        //}
        
    }

public:

    void Init(size_t numNodes) {
        nodes.Init(numNodes);
        containers.Init(numNodes);
        
        containers.elements[0].node = 0;
    }

    Location Insert(Node insertNode) {
        Location location = { 0 };
        Insert(insertNode, location, startSum);
        return location;
    }

    void Remove(Location location) {

    }

};

float RandFloat(float max) {
    return ((float)rand() / (float)RAND_MAX - 0.5f) * 2.0f * max;
}

int main() {
    
    using _BoxMap = BoxMap<float, uint, 3, 16.0f>;
    _BoxMap map = {};
    map.Init(100000);

    for (int i = 0; i < 1000; i++) {
        _BoxMap::Node node = {};
        for (int j = 0; j < 3; j++) {
            float a = RandFloat(16), b = RandFloat(16);
            node.box.low[j] = std::min(a, b);
            node.box.high[j] = std::max(a, b);
        }
        node.childContainer = rand();
        map.Insert(node);
    }


    return 0;
}