#include "Math.hlsli"

#define MAX_LOCATION_SIZE 16

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

Intersection Intersects(float3 origin, float3 ray, Box box){
    float3 invRay = 1.0f / ray;
    Intersection result;
    result.sMin = -1.#INF;
    result.sMax = 1.#INF;
    [unroll(3)] for (uint i = 0; i < 3; i++){
        float sdLow = (box.low[i] - origin[i]) * invRay[i];
        float sdHigh = (box.high[i] - origin[i]) * invRay[i];
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
    TraceResult result;
    result.scale = 1.#INF;
    result.depth = 0;
    
    Container container;
    Node node;
    Intersection intersection;
    uint curInd = 0;
    //float3 curOffset = 0.0f;
    bool newContainer = true;
    
    uint location[MAX_LOCATION_SIZE];
    uint curDepth = 0;
    
    [loop] while (true) {
        if (curDepth == MAX_LOCATION_SIZE)
            break;
        if (curDepth > result.depth)
            result.depth = curDepth;
        container = containers[curInd];
        node = nodes[container.node];
        
        if (newContainer) {
            //curOffset += container.offset;
            location[curDepth] = curInd;
            origin -= container.offset;
            
            intersection = Intersects(origin, ray, node.box);
            if (intersection.sMin != -1.#INF){
                if (intersection.sMin < result.scale){
                    if (node.type == NODE_TYPE_OBJECT){
                        result.scale = intersection.sMin;
                        result.ind = node.child;
                    }
                    else {
                        curInd = node.child;
                        newContainer = true;
                        
                        curDepth++;
                        continue;
                    }
                }
            }
        }
        //curOffset -= container.offset;
        origin += container.offset;
        if (container.sibling != noInd) {
            curInd = container.sibling;
            newContainer = true;
            continue;
        }
        if (curDepth == 0)
            break;
        curInd = location[curDepth - 1];
        newContainer = false;
        curDepth--;
    }
    return result;
}