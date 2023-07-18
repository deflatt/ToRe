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

static uint numIts = 0;
TraceResult Trace(float3 origin, float3 ray){
    const float3 invRay = 1.0f / ray;
    const bool3 raySign = ray < 0.0f;
    
    TraceResult result;
    result.scale = 1.#INF;
    result.ind = noInd;
    
    bool newContainer = true;

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
        
        Container parentContainer = containers[curInd];
        Node parentNode = nodes[parentContainer.node];
        
        if (newContainer){
            curOffset += parentContainer.offset;
            [unroll(3)] for (uint i = 0; i < 3; i++)
                links[curDepth][i] = parentNode.childLink[raySign[i]][i];
        }
        
        uint minDim = noInd;
        float minScale = 1.#INF;
        [unroll(3)] for (uint i = 0; i < 3; i++) {
            [loop] for (uint ind = links[curDepth][i]; ind != noInd; ind = containers[ind].siblingLink[raySign[i]][i], links[curDepth][i] = ind){
                numIts++;
                Container container = containers[ind];
                Node node = nodes[container.node];
                
                float curScale = (curOffset[i] + container.offset[i] + node.box[raySign[i]][i] - origin[i]) * invRay[i];
                
                if (curScale >= result.scale)
                    continue;
                
                curScale = max(0.0f, curScale);
                float3 pos = origin + ray * curScale;
                if (curScale > 0.0f)
                    pos[i] = curOffset[i] + container.offset[i] + node.box[raySign[i]][i];
                
                if (!(all(pos >= curOffset + container.offset + node.box[0]) && all(pos <= curOffset + container.offset + node.box[1])))
                    continue;
                
                
                if (node.type == NODE_TYPE_OBJECT) {
                    if (curScale < lim)
                        continue;
                    result.ind = node.childLink[0][0];
                    result.scale = curScale;
                    result.pos = pos;
                    result.normal = 0.0f;
                    result.normal[i] = raySign[i] ? 1.0f : -1.0f;
                    links[curDepth][i] = containers[ind].siblingLink[raySign[i]][i];
                    break;
                }
                else {
                    if (curScale < minScale){
                        minScale = curScale;
                        minDim = i;
                    }
                    break;
                }
            }
        }
        if (minDim == noInd) {
            if (curDepth == 0)
                break;
            curOffset -= parentContainer.offset;
            newContainer = false;
            curDepth--;
        }
        else {
            uint nextInd = links[curDepth][minDim];
            [unroll(3)] for (uint i = 0; i < 3; i++) {
                if (links[curDepth][i] == nextInd)
                    links[curDepth][i] = containers[links[curDepth][i]].siblingLink[raySign[i]][i];
            }
            location[curDepth + 1] = nextInd;
            newContainer = true;
            curDepth++;
        }
    }
    return result;
}