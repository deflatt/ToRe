#include "Ray_light.hlsli"

float4 main(VSOutput vsOutput) : SV_TARGET
{
    pInd = (uint)vsOutput.position.y * camera[0].windowSize.x + (uint)vsOutput.position.x;
    vsOutput.sightRay = normalize(vsOutput.sightRay);
    
    LightTrace(camera[0].position, vsOutput.sightRay);
    float3 col = (float)numIts / 1.0f / 255.0f;
    return float4(col, 1.0f);
    return 0.0f; // discard; ?
    
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