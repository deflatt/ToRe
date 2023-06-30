#include "Math.hlsli"

#define MAX_LOCATION_SIZE 24

static const uint noInd = -1;

struct Container {
    float3 offset;
    uint node;
    uint sibling;
};
StructuredBuffer<Container> containers : register(t1);

#define NodeType int
#define NODE_TYPE_ROOT 0
#define NODE_TYPE_HELPER 1
#define NODE_TYPE_OBJECT 2

struct Box {
    float3 low;
    float3 high;
};

struct Node {
    Box box;
    uint child;
    NodeType type;
};
StructuredBuffer<Node> nodes : register(t2);

struct TraceResult {
    float scale;
    uint depth;
    uint ind;
};
struct Intersection {
    float sMin, sMax;
};

struct Material {
    float3 emission;
};
StructuredBuffer<Material> materials : register(t3);

Intersection Intersects(float3 origin, float3 invRay, Box box, float3 boxOffset){
    Intersection result;
    result.sMin = -1.#INF;
    result.sMax = 1.#INF;
    [unroll(3)] for (uint i = 0; i < 3; i++){
        float sdLow = (boxOffset[i] + box.low[i] - origin[i]) * invRay[i];
        float sdHigh = (boxOffset[i] + box.high[i] - origin[i]) * invRay[i];
        result.sMin = max(result.sMin, min(sdLow, sdHigh));
        result.sMax = min(result.sMax, max(sdLow, sdHigh));
    }
    result.sMin = max(result.sMin, 0.0f);
    if (result.sMin > result.sMax){
        result.sMin = -1.#INF;
        result.sMax = 1.#INF;
    }
    return result;
}

TraceResult Trace(float3 origin, float3 ray){
    const float3 invRay = 1.0f / ray;
    
    TraceResult result;
    result.scale = 1.#INF;
    result.depth = 0;
    
    Container container;
    Node node;
    Intersection intersection;
    bool newContainer = true;
    
    uint location[MAX_LOCATION_SIZE];
    location[0] = 0;
    uint curDepth = 0;
    
    float3 curOffset = 0.0f;
    
    [loop] while (true) {
        if (curDepth == MAX_LOCATION_SIZE)
            break;
        if (curDepth > result.depth)
            result.depth = curDepth;
        
        uint curInd = location[curDepth];
        
        container = containers[curInd];
        node = nodes[container.node];
        
        if (newContainer) {
            //origin -= container.offset;
            curOffset += container.offset;
        }
        
        float minScale = -1.#INF;
        if (!newContainer)
            minScale = Intersects(origin, invRay, nodes[containers[location[curDepth + 1]].node].box, curOffset + containers[location[curDepth + 1]].offset).sMin;
        float maxScale = result.scale;
        
        uint nextInd = noInd;
        [loop] for (uint childInd = node.child; childInd != noInd; childInd = containers[childInd].sibling){
            intersection = Intersects(origin, invRay, nodes[containers[childInd].node].box, curOffset + containers[childInd].offset);
            if (intersection.sMin == -1.#INF)
                continue;
            if (intersection.sMin < minScale || intersection.sMin > maxScale)
                continue;
            if (intersection.sMin == minScale && (!newContainer && childInd <= location[curDepth + 1]))
                continue;
            if (intersection.sMin == maxScale && nextInd != noInd && childInd > nextInd)
                continue;
            if (nodes[containers[childInd].node].type == NODE_TYPE_OBJECT) {
                if (intersection.sMin < result.scale) {
                    result.scale = intersection.sMin;
                    result.ind = nodes[containers[childInd].node].child;
                }
                continue;
            }
            maxScale = intersection.sMin;
            nextInd = childInd;
        }
        if (nextInd != noInd) {
            location[curDepth + 1] = nextInd;
            newContainer = true;
            curDepth++;
        }
        else {
            if (curDepth == 0)
                break;
            newContainer = false;
            curOffset -= container.offset;
            curDepth--;
        }
    }
    return result;
}