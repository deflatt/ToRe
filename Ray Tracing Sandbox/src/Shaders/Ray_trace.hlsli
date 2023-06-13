#include "Math.hlsli"

static const uint noInd = -1;

struct Container {
    uint node;
    uint siblingContainer;
    uint parentContainer;
    bool locked;
};
StructuredBuffer<Container> containers : register(t1);

struct Box {
    float3 low;
    float3 high;
};

struct Node {
    Box box;
    uint childContainer;
};
StructuredBuffer<Node> nodes : register(t2);

struct TraceResult {
    float scale;
};

bool Intersects(float3 origin, float3 ray, Box box, out float scale){
    float3 invRay = 1.0f / ray;
    float sMin = -1.#INF;
    float sMax = 1.#INF;
    [unroll(3)] for (uint i = 0; i < 3; i++){
        float sdLow = (box.low[i] - origin[i]) * invRay[i];
        float sdHigh = (box.high[i] - origin[i]) * invRay[i];
        sMin = max(sMin, min(sdLow, sdHigh));
        sMax = min(sMax, max(sdLow, sdHigh));
    }
    scale = max(0.0f, sMin);
    return (sMin <= sMax) && sMax > 0.0f;
}

TraceResult Trace(float3 origin, float3 ray){
    TraceResult result;
    result.scale = 1.#INF;
    
    bool newContainer = true;
    uint depth = 0;
    uint curContainer = 0;
    
    [loop] while (true) {
        Container container = containers[curContainer];
        Node node = nodes[container.node];
        if (newContainer){
            float curScale;
            if (Intersects(origin, ray, node.box, curScale)){
                if (curScale < result.scale) {
                    if (container.locked) {
                        result.scale = curScale;
                    }
                    else {
                        curContainer = node.childContainer;
                        newContainer = true;
                        continue;
                    }
                }
            }
        }
        if (container.siblingContainer != noInd) {
            curContainer = container.siblingContainer;
            newContainer = true;
            continue;
        }
        curContainer = container.parentContainer;
        if (curContainer == noInd)
            break;
        newContainer = false;
    }
    
    return result;
}