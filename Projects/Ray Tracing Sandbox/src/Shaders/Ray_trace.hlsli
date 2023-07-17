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
    const bool3 raySign = ray < 0.0f;
    
    TraceResult result;
    result.scale = 1.#INF;
    result.ind = noInd;
    
    Container parentContainer;
    Node parentNode, node;
    bool newContainer = true;
    Intersection intersection;
    
    uint location[MAX_LOCATION_SIZE];
    location[0] = 0;
    uint curDepth = 0;
    uint3 links[MAX_LOCATION_SIZE];
    
    float3 curOffset = 0.0f;
    
    uint curInd;
    
    [loop] while (true) {
        if (curDepth == MAX_LOCATION_SIZE)
            break;
        
        curInd = location[curDepth];
        
        parentContainer = containers[curInd];
        parentNode = nodes[parentContainer.node];
        
        if (newContainer){
            curOffset += parentContainer.offset;
            [unroll(3)] for (uint i = 0; i < 3; i++)
                links[curDepth][i] = parentNode.childLink[raySign[i]][i];
        }
        [loop] while (true) {
            uint minDim = noInd;
            float minScale = 1.#INF;
            [unroll(3)] for (uint i = 0; i < 3; i++) {
                uint ind = links[curDepth][i];
                if (ind == noInd)
                    continue;
                float curScale = (curOffset[i] + containers[ind].offset[i] + nodes[containers[ind].node].box[raySign[i]][i] - origin[i]) * invRay[i];
                if (curScale < minScale && curScale < result.scale){
                    minScale = curScale;
                    minDim = i;
                }
            }
            if (minDim == noInd){
                if (curDepth == 0)
                    return result;
                curOffset -= parentContainer.offset;
                newContainer = false;
                curDepth--;
                break;
            }
            uint minInd = links[curDepth][minDim];
            
            links[curDepth][minDim] = containers[minInd].siblingLink[raySign[minDim]][minDim];
            
            minScale = max(0.0f, minScale);
            float3 pos = origin + ray * minScale;
            if (minScale > 0.0f)
                pos[minDim] = curOffset[minDim] + containers[minInd].offset[minDim] + nodes[containers[minInd].node].box[raySign[minDim]][minDim];
            
            if (!(all(pos >= curOffset + containers[minInd].offset + nodes[containers[minInd].node].box[0]) && all(pos <= curOffset + containers[minInd].offset + nodes[containers[minInd].node].box[1])))
                continue;
            node = nodes[containers[minInd].node];
            if (node.type == NODE_TYPE_OBJECT){
                if (minScale < lim)
                    continue;
                result.ind = node.childLink[0][0];
                result.scale = minScale;
                result.pos = pos;
                result.normal = 0.0f;
                result.normal[minDim] = raySign[minDim] ? 1.0f : -1.0f;
                continue;
            }
            else {
                location[curDepth + 1] = minInd;
                newContainer = true;
                curDepth++;
                break;
            }
        }
    }
    //if (result.ind != noInd)
    //    result.pos = origin + ray * result.scale;
    return result;
}