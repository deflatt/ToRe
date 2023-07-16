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
    
    float3 curOffset = 0.0f;
    
    uint curInd;
    float minScale;
    float maxScale;
    
    [loop] while (true) {
        if (curDepth == MAX_LOCATION_SIZE)
            break;
        
        curInd = location[curDepth];
        
        container = containers[curInd];
        node = nodes[container.node];
        
        if (newContainer){
            curOffset += container.offset;
            
            intersection = Intersects(origin, invRay, node.box, curOffset);
            if (intersection.sMin != -1.#INF){
                if (intersection.sMin < result.scale){
                    if (node.type == NODE_TYPE_OBJECT){
                        result.scale = intersection.sMin;
                        result.ind = node.childLink[0][0];
                        result.normal = intersection.normal;
                        result.pos = origin + ray * result.scale;
                    }
                    else {
                        uint nextInd = node.childLink[0][0];
                        location[curDepth + 1] = nextInd;
                        newContainer = true;
                        curDepth++;
                        continue;
                    }
                }
            }
        }
        curOffset -= container.offset;
        
        if (container.siblingLink[0][0] != noInd){
            location[curDepth] = container.siblingLink[0][0];
            newContainer = true;
            continue;
        }
        
        if (curDepth == 0)
            break;
        newContainer = false;
        curDepth--;
    }
    if (result.ind != noInd)
        result.pos = origin + ray * result.scale;
    return result;
}