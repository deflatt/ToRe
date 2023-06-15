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

    struct Node {
        Box box = {};
        T_ind childContainer = noInd;
    };

    struct Container {
        T_ind node = noInd;
        T_ind siblingContainer = noInd;
        T_ind parentContainer = noInd;
        Bool locked = false; // Move to Node?
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

//protected:

    List<Node> nodes = {};
    List<Container> containers = {};

    T_ind AddChildContainer(T_ind parentContainerInd) {
        Node* parent = &nodes.elements[containers.elements[parentContainerInd].node];
        T_ind containerInd = containers.New();
        Container* container = &containers.elements[containerInd];
        container->siblingContainer = parent->childContainer;
        container->parentContainer = parentContainerInd;
        parent->childContainer = containerInd;
        return containerInd;
    }

    T_ind AddChild(T_ind parentContainerInd) {
        T_ind containerInd = AddChildContainer(parentContainerInd);
        Container* container = &containers.elements[containerInd];
        container->node = nodes.New();
        return containerInd;
    }

    void Insert(Node insertNode, Location& location, T_val maxSum) {
        Container* parentContainer = &containers.elements[location.back()];
        Node* parent = &nodes.elements[parentContainer->node];

        parent->box.Fit(insertNode.box);

        if (insertNode.box.GetSum() <= maxSum / divisor) {
            for (T_ind childInd = parent->childContainer; childInd != noInd; childInd = containers.elements[childInd].siblingContainer) {
                Container* childContainer = &containers.elements[childInd];
                if (childContainer->locked)
                    continue;
                Node* child = &nodes.elements[childContainer->node];

                T_val sum = child->box.GetFit(insertNode.box).GetSum();
                if (sum <= maxSum / divisor) {
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
        // Will require a full Location once compression has been added

        Container* container = &containers.elements[location.back()];
        if (!container->locked) {
            throw E_BoxMapBadRemoval();
        }
        Container* parentContainer = &containers.elements[container->parentContainer];
        Node* parent = &nodes.elements[parentContainer->node];
        if (parent->childContainer == location.back()) {
            parent->childContainer = container->siblingContainer;
            std::cout << "Set parent to " << parent->childContainer << std::endl;
        }
        else {
            for (T_ind child = nodes.elements[parentContainer->node].childContainer;; child = containers.elements[child].siblingContainer) {
                Container* childContainer = &containers.elements[child];
                if (childContainer->siblingContainer == noInd) {
                    throw E_BoxMapBadLocation();
                }
                else if (childContainer->siblingContainer == location.back()) {
                    childContainer->siblingContainer = container->siblingContainer;
                    break;
                }
            }
        }
        nodes.Remove(container->node);
        containers.Remove(location.back());
    }

};

