#include "Ray_trace.hlsli"
#include "Ray_sky.hlsli"

#define numBounces 1
#define numSamples 1

void LightTrace(float3 origin, float3 ray){
    TraceResult inTrace = Trace(origin, ray);
    
    if (inTrace.ind == noInd){
        pixelInfo[pInd].ind = noInd;
        pixelInfo[pInd].pos = 0.0f;
        pixelInfo[pInd].normal = 0.0f;
        pixelInfo[pInd].incomingLight = SkyTrace(ray);
        return;
    }
    //pixelInfo[pInd].ind = inTrace.ind;
    //pixelInfo[pInd].pos = 0.0f;
    //pixelInfo[pInd].normal = 0.0f;
    //pixelInfo[pInd].incomingLight = 0.0f;
    //return;
    float3 total = 0.0f;
    for (uint i = 0; i < numSamples; i++){
        
        float3 reflection = float3(1.0f, 1.0f, 1.0f);
        float3 curOrigin = inTrace.pos;
        float3 curRay;
        float3 curNormal = inTrace.normal;
        
        for (uint b = 0; b < numBounces; b++){
            curRay = RandSurfaceDir(curNormal);
            TraceResult trace = Trace(curOrigin, curRay);
            if (trace.ind == noInd) {
                total += SkyTrace(curRay) * reflection;
                break;
            }
            Material mat = materials[trace.ind];
            total += mat.emission * reflection * dot(curNormal, curRay);
            reflection *= mat.reflection;
            
            curOrigin = trace.pos;
            curNormal = trace.normal;
        }
    }
    total /= (float)numSamples;
    pixelInfo[pInd].ind = inTrace.ind;
    pixelInfo[pInd].pos = inTrace.pos;
    pixelInfo[pInd].normal = inTrace.normal;
    pixelInfo[pInd].incomingLight = total;
    
    //for (uint i = 0; i <= numBounces; i++){
    //    TraceResult trace = Trace(origin, ray);
    //    if (trace.ind == noInd){
    //        result += SkyTrace(ray) * reflection;
    //        break;
    //    }
    //    Material mat = materials[trace.ind];
    //    result += mat.emission * reflection * dot(normal, ray);
    //    reflection *= mat.reflection;
        
    //    origin = trace.pos;
    //    ray = RandSurfaceDir(trace.normal);
    //}
}