#include "Ray_math.hlsli"

struct TraceResult {
    float scale;
    uint ind;
    float3 pos;
    float3 normal;
};

static uint numIts = 0;
TraceResult Trace(float3 origin, float3 ray){
    const float3 invRay = 1.0f / ray;
    const bool3 raySign = (ray < 0.0f);
    
    TraceResult result;
    result.scale = 1.#INF;
    result.ind = noInd;
    result.pos = 0.0f;
    result.normal = 0.0f;
    
    Container container;
    Node node;
    Node childNode;
    
    bool newContainer = true;
    float3 curOffset = 0.0f;
    uint location[MAX_LOCATION_SIZE];
    location[0] = 0;
    uint curDepth = 0;
    uint3 links[MAX_LOCATION_SIZE];
    
    [loop] while (true) {
        container = containers[location[curDepth]];
        node = nodes[container.node];
        
        if (newContainer){
            curOffset += container.offset;
            for (uint dim = 0; dim < 3; dim++)
                links[curDepth][dim] = node.childLink[raySign[dim]][dim];   
        }
        
        float minScale = 1.#INF;
        uint minDim = noInd;
        for (uint dim = 0; dim < 3; dim++){
            for (uint childInd = links[curDepth][dim]; childInd != noInd; childInd = containers[childInd].siblingLink[raySign[dim]][dim], links[curDepth][dim] = childInd){
                numIts++;
                childNode = nodes[containers[childInd].node];
                
                childNode.box[0] += curOffset + containers[childInd].offset;
                childNode.box[1] += curOffset + containers[childInd].offset;
            
                float curScale = (childNode.box[raySign[dim]][dim] - origin[dim]) * invRay[dim];
                curScale = max(curScale, 0.0f);
                if (curScale >= result.scale)
                    break;
            
                float3 pos = origin + ray * curScale;
                if (curScale > 0.0f)
                    pos[dim] = childNode.box[raySign[dim]][dim];
            
                if (!(all(pos >= childNode.box[0]) && all(pos <= childNode.box[1])))
                    continue;

                if (childNode.type == NODE_TYPE_OBJECT) {
                    if (curScale < lim)
                        continue;
                    result.ind = childNode.childLink[0][0];
                    result.scale = curScale;
                    result.pos = pos;
                    result.normal = 0.0f;
                    result.normal[dim] = raySign[dim] ? 1.0f : -1.0f;
                    break;
                }
                else {
                    if (curScale < minScale) {
                        minScale = curScale;
                        minDim = dim;
                    }
                    break;
                }
            }
        }
        if (minDim != noInd){
            if (curDepth + 1 == MAX_LOCATION_SIZE){
                result.ind = noInd;
                break;
            }
            location[curDepth + 1] = links[curDepth][minDim];
            links[curDepth][minDim] = containers[links[curDepth][minDim]].siblingLink[raySign[minDim]][minDim];
            newContainer = true;
            curDepth++;
        }
        else {
            if (curDepth == 0)
                break;
            curOffset -= container.offset;
            newContainer = false;
            curDepth--;
        }
    }
    return result;
}