#include "Ray_math.hlsli"

struct TraceResult {
    float scale;
    uint ind;
    float3 pos;
    float3 normal;
};
struct Intersection {
    float sMin, sMax;
    float3 normal;
};

Intersection Intersects(float3 origin, float3 invRay, Box box, float3 boxOffset){
    Intersection result;
    result.sMin = -1.#INF;
    result.sMax = 1.#INF;
    result.normal = 0.0f;
    uint side = 0;
    [unroll(3)] for (uint i = 0; i < 3; i++){
        float sdLow = (boxOffset[i] + box.low[i] - origin[i]) * invRay[i];
        float sdHigh = (boxOffset[i] + box.high[i] - origin[i]) * invRay[i];
        float sdMin = min(sdLow, sdHigh);
        float sdMax = max(sdLow, sdHigh);

        if (sdMin > result.sMin){
            result.sMin = sdMin;
            side = i;
        }
        if (sdMax < result.sMax){
            result.sMax = sdMax;
        }
    }
    result.sMin = max(result.sMin, lim);
    if (result.sMin > result.sMax){
        result.sMin = -1.#INF;
        result.sMax = 1.#INF;
    }
    else {
        result.normal[side] = (invRay[side] > 0.0f) ? -1.0f : 1.0f;
    }
    return result;
}

TraceResult Trace(float3 origin, float3 ray){
    const float3 invRay = 1.0f / ray;
    
    TraceResult result;
    result.scale = 1.#INF;
    result.ind = noInd;
    
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
        
        uint curInd = location[curDepth];
        
        container = containers[curInd];
        node = nodes[container.node];
        
        if (newContainer) {
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
            if (intersection.sMin < minScale || intersection.sMin > maxScale
                || (intersection.sMin == minScale && (!newContainer && childInd <= location[curDepth + 1]))
                || (intersection.sMin == maxScale && nextInd != noInd && childInd > nextInd))
                continue;
            if (nodes[containers[childInd].node].type == NODE_TYPE_OBJECT) {
                if (intersection.sMin < result.scale) {
                    result.scale = intersection.sMin;
                    result.ind = nodes[containers[childInd].node].child;
                    result.normal = intersection.normal;
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
    if (result.ind != noInd)
        result.pos = origin + ray * result.scale;
    return result;
}