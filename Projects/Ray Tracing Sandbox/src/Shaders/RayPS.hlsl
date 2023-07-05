#include "Ray_trace.hlsli"

struct VSOutput {
    float4 position : SV_Position;
    float3 sightRay : SIGHT_RAY;
};

struct Camera {
    float3 position;
    float2 rotation;
    float fov;
    float aspectRatio;
};
StructuredBuffer<Camera> camera : register(t0);

float4 main(VSOutput vsOutput) : SV_TARGET
{
    vsOutput.sightRay = normalize(vsOutput.sightRay);
    TraceResult t = Trace(camera[0].position, vsOutput.sightRay);
    //return (float)t.checks / 10.0f / 255.0f;
    if (t.scale == 1.#INF)
        return float4(0.0f, 0.0f, 1.0f, 1.0f);
    else
        return float4(materials[t.ind].emission * (1.0f / (1.0f + 0.25f * sqrt(t.scale))), 1.0f);
}