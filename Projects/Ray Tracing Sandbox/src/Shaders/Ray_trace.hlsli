#include "Ray_math.hlsli"

struct TraceResult {
    float scale;
    uint ind;
    float3 pos;
    float3 normal;
};


struct Intersection {
    float sMin;
    float3 pos;
    float3 normal;
};

static uint numIntersectionChecks = 0;
Intersection Intersects(float3 origin, float3 ray, float3 invRay, Box box, float3 boxOffset){
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
        result.pos = origin + ray * result.sMin;
        result.pos[side] = boxOffset[side] + ((invRay[side] > 0.0f) ? box.low[side] : box.high[side]);
        result.normal[side] = (invRay[side] > 0.0f) ? -1.0f : 1.0f;
    }
    return result;
}

TraceResult Trace(float3 origin, float3 ray){
    const float3 invRay = 1.0f / ray;
    //const bool3 raySign = ray < 0.0f;
    
    bool linkSign;
    uint linkDim;
    {
        float rMax = -1.#INF;
        [unroll(3)] for (uint i = 0; i < 3; i++) {
            if (abs(ray[i]) > rMax){
                rMax = abs(ray[i]);
                linkSign = ray[i] < 0.0f;
                linkDim = i;
            }
        }
    }
    
    TraceResult result;
    result.scale = 1.#INF;
    result.ind = noInd;
    
    bool newContainer = true;

    uint location[MAX_LOCATION_SIZE];
    location[0] = 0;
    uint curDepth = 0;
    
    float3 curOffset = 0.0f;
    uint curInd;
    
    [loop] while (true) {
        if (curDepth == MAX_LOCATION_SIZE)
            break;
        
        curInd = location[curDepth];

        Container container = containers[curInd];
        Node node = nodes[container.node];
        
        if (newContainer){
            curOffset += container.offset;
            
            Intersection intersection = Intersects(origin, ray, invRay, node.box, curOffset);
            if (intersection.sMin != -1.#INF && intersection.sMin < result.scale){
                if (node.type == NODE_TYPE_OBJECT){
                    result.ind = node.childLink[0][0];
                    result.scale = intersection.sMin;
                    result.pos = intersection.pos;
                    result.normal = intersection.sMin;
                }
                else {
                    location[curDepth + 1] = node.childLink[linkSign][linkDim];
                    newContainer = true;
                    curDepth++;
                    continue;
                }
            }
        }
        curOffset -= container.offset;
        
        if (container.siblingLink[linkSign][linkDim] != noInd) {
            location[curDepth] = container.siblingLink[linkSign][linkDim];
            newContainer = true;
            continue;
        }
        
        if (curDepth == 0)
            break;
        newContainer = false;
        curDepth--;
    }
    return result;
}