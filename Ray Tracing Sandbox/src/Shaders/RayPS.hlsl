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

struct Rect {
    float3 low;
    float3 high;
};

bool Intersects(float3 origin, float3 ray, Rect rect){
    float3 invRay = 1.0f / ray;
    
    float sMin = -1.#INF;
    float sMax = 1.#INF;
    [unroll(3)] for (uint i = 0; i < 3; i++){
        float sdLow = (rect.low[i] - origin[i]) * invRay[i];
        float sdHigh = (rect.high[i] - origin[i]) * invRay[i];
        sMin = max(sMin, min(sdLow, sdHigh));
        sMax = min(sMax, max(sdLow, sdHigh));
    }
    return (sMin <= sMax) && sMax > 0.0f;
}

bool TestIntersect(float3 origin, float3 ray, Rect rect)
{
    [loop] for (uint i = 0; i < 10000; i++){
        if (Intersects(origin, ray, rect))
            return true;
    }
    return false;
}

float4 main(VSOutput vsOutput) : SV_TARGET
{
    Rect rect;
    rect.low = float3(-0.5f, -0.5f, -0.5f);
    rect.high = float3(0.5f, 0.5f, 0.5f);
    
    if (TestIntersect(camera[0].position, vsOutput.sightRay, rect))
        return float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    return float4(vsOutput.sightRay, 1.0f);
}