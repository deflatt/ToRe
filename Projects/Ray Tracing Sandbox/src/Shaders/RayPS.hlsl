#include "Ray_light.hlsli"

float4 main(VSOutput vsOutput) : SV_TARGET
{
    pInd = (uint)vsOutput.position.y * camera[0].windowSize.x + (uint)vsOutput.position.x;
    vsOutput.sightRay = normalize(vsOutput.sightRay);
    
    LightTrace(camera[0].position, vsOutput.sightRay);
    //return 0.0f; // discard; ?
    float3 col = (float)numIts / 1.0f / 255.0f;
    return float4(col, 1.0f);
    
    //TraceResult result = Trace(camera[0].position, vsOutput.sightRay);
    //if (result.ind == noInd){
    //    return float4(SkyTrace(vsOutput.sightRay), 1.0f);
    //}
    //return float4(result.debugCol, 1.0f);
    //return float4(materials[result.ind].reflection, 1.0f);
    //float3 col = (float)numIts / 255.0f;
    //return float4(abs(result.normal), 1.0f);
    //return float4(col, 1.0f);
    //return float4((float)result.ind / 255.0f, (float)(result.ind - 256) / 255.0f, (float)(result.ind - 512) / 255.0f, 1.0f);
    //return float4(result.scale, result.scale / 2.0f, result.scale / 4.0f, 1.0f);
    //return float4(result.node.box[1] / 4.0f, 1.0f);
    
    //TraceResult t = Trace(camera[0].position, vsOutput.sightRay);
    ////return (float)t.numChecks / 10.0f / 255.0f;
    //if (t.scale == 1.#INF)
    //    return float4(SkyTrace(vsOutput.sightRay), 1.0f);
    //else
    //    return float4(materials[t.ind].emission * (1.0f / (1.0f + 0.25f * sqrt(t.scale))), 1.0f);
    
    //{
    //    float mul = 1.0f;
    //    {
    //        float3 p = camera[0].position + vsOutput.sightRay * t.scale;
    //        p.y += 0.01f;
    //        TraceResult t2 = Trace(p, float3(1.0f, 2.0f, 0.5f));
    //        if (t2.ind != noInd)
    //            mul = 0.5f;
    //    }
    //    return float4(materials[t.ind].emission * (1.0f / (1.0f + 0.25f * sqrt(t.scale))) * mul, 1.0f);
    //    
    //}
}