#include "Math.hlsli"

static const uint noInd = -1;

struct Container {
    float3 offset;
    uint node;
    uint sibling;
    uint parent;
};
StructuredBuffer<Container> containers : register(t1);

#define NODE_TYPE_ROOT 0
#define NODE_TYPE_NODE 1
#define NODE_TYPE_OBJECT 2

struct Node {
    float3 size;
    uint child;
    int type;
};
StructuredBuffer<Node> nodes : register(t2);

struct TraceResult {
    float scale;
    uint depth;
};
struct Intersection {
    float sMin, sMax;
};

struct Box {
    float3 low;
    float3 high;
};

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
    
    // Gonna have to track full location once compression has been added
    
    uint curDepth = 0;
    [loop] while (true) {
        if (curDepth > result.depth)
            result.depth = curDepth;
        container = containers[curInd];
        node = nodes[container.node];
        
        if (newContainer) {
            //curOffset += container.offset;
            origin -= container.offset;
            
            Box box;
            box.low = 0.0f;
            box.high = box.low + node.size;
            intersection = Intersects(origin, ray, box);
            
            if (intersection.sMin != -1.#INF){
                if (intersection.sMin < result.scale){
                    if (node.type == NODE_TYPE_OBJECT){
                        result.scale = intersection.sMin;
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
            
            curDepth++;
            continue;
        }
        if (container.parent == noInd)
            break;
        curInd = container.parent;
        newContainer = false;
        curDepth--;
    }
    return result;
}