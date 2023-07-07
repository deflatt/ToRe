#include "Ray_math.hlsli"

struct TraceResult {
    float scale;
    uint ind;
    float3 pos;
    float3 normal;
};
struct Intersection {
    float sMin;
    float3 normal;
};

static uint numIntersectionChecks = 0;
Intersection Intersects(float3 origin, float3 invRay, Box box, float3 boxOffset){
    numIntersectionChecks++;

    Intersection result;
    result.sMin = -1.#INF;
    float sMax = 1.#INF;
    result.normal = 0.0f;
    uint side = 0;
    
    float3 sLow = (boxOffset + box.low - origin) * invRay;
    float3 sHigh = (boxOffset + box.high - origin) * invRay;
    [unroll(3)] for (uint i = 0; i < 3; i++){
        float sdMin = min(sLow[i], sHigh[i]);
        float sdMax = max(sLow[i], sHigh[i]);
        
        if (sdMin > result.sMin){
            result.sMin = sdMin;
            side = i;
        }
        if (sdMax < sMax){
            sMax = sdMax;
        }
    }
    result.sMin = max(result.sMin, lim);
    if (result.sMin > sMax){
        result.sMin = -1.#INF;
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
    bool newContainer = true;
    Intersection intersection;
    
    uint location[MAX_LOCATION_SIZE];
    location[0] = 0;
    uint curDepth = 0;
    float minScales[MAX_LOCATION_SIZE];
    
    float3 curOffset = 0.0f;
    
    uint curInd, lastChildInd;
    float minScale;
    float maxScale;
    
    [loop] while (true) {
        if (curDepth == MAX_LOCATION_SIZE)
            break;
        
        curInd = location[curDepth];
        if (!newContainer)
            lastChildInd = location[curDepth + 1];
        
        container = containers[curInd];
        node = nodes[container.node];
        
        if (newContainer) {
            curOffset += container.offset;
        }
        
        minScale = -1.#INF;
        if (!newContainer)
            minScale = minScales[curDepth];
            //minScale = Intersects(origin, invRay, nodes[containers[lastChildInd].node].box, curOffset + containers[lastChildInd].offset).sMin;
        
        maxScale = result.scale;
        
        uint nextInd = noInd;
        [loop] for (uint childInd = node.child; childInd != noInd; childInd = containers[childInd].sibling){
            if (!newContainer && childInd == lastChildInd)
                continue;
            intersection = Intersects(origin, invRay, nodes[containers[childInd].node].box, curOffset + containers[childInd].offset);
            if (intersection.sMin == -1.#INF)
                continue;
            if (intersection.sMin < minScale || intersection.sMin > maxScale
                || (intersection.sMin == minScale && (!newContainer && childInd <= lastChildInd))
                || (intersection.sMin == maxScale && nextInd != noInd && childInd > nextInd))
                continue;
            if (nodes[containers[childInd].node].type == NODE_TYPE_OBJECT) {
                if (intersection.sMin < result.scale) {
                    result.scale = intersection.sMin;
                    result.ind = nodes[containers[childInd].node].child;
                    result.normal = intersection.normal;
                    result.pos = origin + ray * result.scale;
                }
                continue;
            }
            maxScale = intersection.sMin;
            nextInd = childInd;
        }
        if (nextInd != noInd) {
            location[curDepth + 1] = nextInd;
            minScales[curDepth] = maxScale;
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