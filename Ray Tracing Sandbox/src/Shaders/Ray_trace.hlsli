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
struct Intersection {
    float sMin, sMax;
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
    
    float curScale = 0.0f;
    
    [loop] while (true) {
        float newScale = 1.#INF;
        
        uint curContainer = 0;
        bool newContainer = true;
        [loop] while (true) {
            if (newContainer) {
                Intersection intersection = Intersects(origin, ray, nodes[containers[curContainer].node].box);
                if (intersection.sMin >= 0.0f && intersection.sMin < newScale){
                    if (intersection.sMin > curScale) {
                        newScale = intersection.sMin;
                    }
                    else if (intersection.sMax > curScale && !containers[curContainer].locked) {
                        curContainer = nodes[containers[curContainer].node].childContainer;
                        newContainer = true;
                        continue;
                    }
                }
            }
            if (containers[curContainer].siblingContainer != noInd) {
                curContainer = containers[curContainer].siblingContainer;
                newContainer = true;
                continue;
            }
            curContainer = containers[curContainer].parentContainer;
            if (curContainer == noInd)
                break;
            newContainer = false;
        }
        curScale = newScale;
        if (curScale == 1.#INF)
            break;
        
        curContainer = 0;
        newContainer = true;
        [loop] while (true) {
            if (newContainer) {
                Intersection intersection = Intersects(origin, ray, nodes[containers[curContainer].node].box);
                if (intersection.sMin >= 0.0f && intersection.sMin <= curScale && intersection.sMax >= curScale){
                    if (containers[curContainer].locked) {
                        result.scale = intersection.sMin;
                        break;
                    }
                    else {
                        curContainer = nodes[containers[curContainer].node].childContainer;
                        newContainer = true;
                        continue;
                    }
                }
            }
            if (containers[curContainer].siblingContainer != noInd) {
                curContainer = containers[curContainer].siblingContainer;
                newContainer = true;
                continue;
            }
            curContainer = containers[curContainer].parentContainer;
            if (curContainer == noInd)
                break;
            newContainer = false;
        }
        if (result.scale != 1.#INF)
            break;
    }
    return result;
}