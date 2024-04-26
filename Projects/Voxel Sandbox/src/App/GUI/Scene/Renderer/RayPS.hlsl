struct Camera {
    float3 pos;
    float2 rot;
    float fov;
    float aspectRatio;
};
StructuredBuffer<Camera> cameras : register(t0);

struct VSOutput {
    float4 pos : SV_Position;
    float3 sightRay : SIGHT_RAY;
};

struct AABB {
    float3 lower;
    float3 higher;
};

float4 main(VSOutput input) : SV_TARGET
{
    Camera cam = cameras[0];
    input.sightRay = normalize(input.sightRay);
    AABB bounds = { { 1.0f, 1.0f, 1.0f }, { 2.0f, 2.0f, 2.0f } };
    {
        AABB b = bounds;
        b.lower -= cam.pos;
        b.higher -= cam.pos;
        
        float sMin = -1.#INF, sMax = 1.#INF;
        float3 sLow = b.lower / input.sightRay;
        float3 sHigh = b.higher / input.sightRay;
        for (int i = 0; i < 3; i++)
        {
            float siMin = min(sLow[i], sHigh[i]);
            float siMax = max(sLow[i], sHigh[i]);
            if (siMin > sMin)
                sMin = siMin;
            if (siMax < sMax)
                sMax = siMax;
        }
        if (sMin <= sMax && sMin > 0.0f)
            return 1.0f / sMin;
    }
    
    float3 sunRay = { 1.0f, 1.0f, 1.0f };
    float3 skyCol = { 0.3f, 0.5f, 0.7f };
    float3 sunCol = { 5.0f, 5.0f, 1.0f };
    float a = dot(normalize(sunRay), normalize(input.sightRay));
    float3 col = skyCol + sunCol * atan(a) * 0.1f;
    return float4(col, 1.0f);
}