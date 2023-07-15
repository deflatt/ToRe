#include "Ray_math.hlsli"

struct TraceResult {
    float scale;
    uint ind;
    float3 pos;
    float3 normal;
    float3 debugCol;
};

float Intersects(float3 origin, float3 invRay, Box box) {
    float sMin = -1.#INF;
    float sMax = 1.#INF;

    for (uint i = 0; i < 3; i++){
        float s1 = (box.low[i] - origin[i]) * invRay[i];
        float s2 = (box.high[i] - origin[i]) * invRay[i];
        float sdMin = min(s1, s2);
        float sdMax = max(s1, s2);
        if (sdMin > sMin)
            sMin = sdMin;
        if (sdMax < sMax)
            sMax = sdMax;
    }
    sMin = max(sMin, lim);
    if (sMin > sMax)
        return 1.#INF;
    return sMin;
}

static uint numIts = 0;
TraceResult Trace(float3 origin, float3 ray){
    const float3 invRay = 1.0f / ray;
    const bool3 raySign = (ray < 0.0f);
    
    TraceResult result;
    result.scale = 1.#INF;
    result.ind = noInd;
    result.pos = 0.0f;
    result.normal = 0.0f;
    
    bool newContainer = true;
    float3 curOffset = 0.0f;
    uint location[MAX_LOCATION_SIZE];
    location[0] = 0;
    uint curDepth = 0;
    
    [loop] while (true) {
        Container parentContainer = containers[location[curDepth]];
        Node parentNode = nodes[parentContainer.node];
        
        if (!newContainer && location[curDepth] == 1){
            result.ind = 0;
            if (any(curOffset == 1.0f))
                result.debugCol = float3(0.0f, 1.0f, 0.0f); // + float3(10.0f, 10.0f, 10.0f);
            else
                result.debugCol = float3(1.0f, 0.0f, 0.0f);
            return result;
        }
        
        if (newContainer) {
            curOffset += parentContainer.offset;
            
            Box box = parentNode.box;
            box.low += curOffset;
            box.high += curOffset;
            float curScale = Intersects(origin, invRay, box);
            
            //if (curScale != 1.#INF){
            //    if (location[curDepth] == 5){
            //        if (curScale == 1.#INF)
            //            return result;
            //        result.ind = 0;
            //        result.debugCol = curOffset;
            //        return result;
            //    }
            //}
            
            if (curScale != 1.#INF && curScale < result.scale){
                if (parentNode.type == NODE_TYPE_OBJECT){
                    result.scale = curScale;
                    result.ind = parentNode.childLink[0][0];
                }
                else {
                    if (curDepth + 1 == MAX_LOCATION_SIZE){
                        result.ind = noInd;
                        return result;
                    }
                    location[curDepth + 1] = parentNode.childLink[0][0];
                    curDepth++;
                    newContainer = true;
                    continue;
                }
            }
        }
        curOffset -= parentContainer.offset;
        if (parentContainer.siblingLink[0][0] != noInd){
            location[curDepth] = parentContainer.siblingLink[0][0];
            newContainer = true;
            continue;
        }
        if (curDepth == 0)
            break;
        curDepth--;
        newContainer = false;
    }
    
    
    
    
    
    
    //uint3 links[MAX_LOCATION_SIZE];
    
    //[loop] while (true) {
    //    Container parentContainer = containers[location[curDepth]];
    //    Node parentNode = nodes[parentContainer.node];
        
    //    if (newContainer){
    //        curOffset += parentContainer.offset;
    //        [unroll(3)] for (uint dim = 0; dim < 3; dim++){
    //            links[curDepth][dim] = parentNode.childLink[raySign[dim]][dim];
    //        }
    //    }
        
    //    [loop] while (true) {
    //        uint minDim = noInd;
    //        float minScale = 1.#INF;
    //        [unroll(3)] for (uint dim = 0; dim < 3; dim++){
    //            uint childInd = links[curDepth][dim];
    //            if (childInd == noInd)
    //                continue;
    //            float totalOffset = curOffset[dim] + containers[childInd].offset[dim];
    //            float curScale = (totalOffset + nodes[containers[childInd].node].box[raySign[dim]][dim] - origin[dim]) * invRay[dim];
    //            //curScale = max(curScale, 0.0f);
    //            if (curScale < minScale){
    //                minScale = curScale;
    //                minDim = dim;
    //            }
    //        }
    //        if (minDim == noInd){
    //            if (curDepth == 0)
    //                return result;
    //            curOffset -= parentContainer.offset;
    //            curDepth--;
    //            newContainer = false;
    //            break;
    //        }
            
    //        uint childInd = links[curDepth][minDim];
            
    //        links[curDepth][minDim] = containers[links[curDepth][minDim]].siblingLink[raySign[minDim]][minDim];
            
    //        Node node = nodes[containers[childInd].node];
    //        float3 totalOffset = curOffset + containers[childInd].offset;
            
    //        minScale = max(minScale, 0.0f);
    //        float3 pos = origin + ray * minScale;
    //        if (minScale > 0.0f)
    //            pos[minDim] = totalOffset[minDim] + node.box[raySign[minDim]][minDim];
    //        if (!(all(pos >= totalOffset + node.box[0]) && all(pos <= totalOffset + node.box[1])))
    //            continue;
            
    //        // TODO: Check for duplicate
    //        {
    //            for (uint dim = 0; dim < 3; dim++){
    //                if (dim != minDim && links[curDepth][dim] == childInd)
    //                    links[curDepth][dim] = containers[links[curDepth][dim]].siblingLink[raySign[dim]][dim];
    //            }
    //        }
            
    //        if (node.type == NODE_TYPE_OBJECT){
    //            if (minScale < lim)
    //                continue;
    //            result.ind = node.childLink[0][0];
    //            result.scale = minScale;
    //            result.pos = pos;
    //            result.normal = 0.0f;
    //            result.normal[minDim] = raySign[minDim] ? 1.0f : -1.0f;
    //            result.node = node;
    //            return result;
    //        }
    //        else {
    //            if (curDepth == MAX_LOCATION_SIZE){
    //                return result;
    //            }
    //            location[curDepth + 1] = childInd;
    //            curDepth++;
    //            newContainer = true;
    //            break;
    //        }
    //    }
    //}
    return result;
}