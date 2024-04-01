
struct Camera {
    float3 pos;
    float2 rot;
};
StructuredBuffer<Camera> cameras : register(t0);

struct VSOutput {
    float4 pos : SV_Position;
    float3 sightRay : SIGHT_RAY;
};

struct Bounds {
    float3 lower;
    float3 higher;
};

float4 main(VSOutput input) : SV_Target {
    Camera cam = cameras[0];
    
    input.sightRay = normalize(input.sightRay);
    
    Bounds bounds = { { 1.0f, 1.0f, 1.0f }, { 2.0f, 2.0f, 2.0f } };
    {
        Bounds b = bounds;
        b.lower -= cam.pos;
        b.higher -= cam.pos;
        
        float sMin = -1.#INF, sMax = 1.#INF;
        float3 sLow = b.lower / input.sightRay;
        float3 sHigh = b.higher / input.sightRay;
        for (int i = 0; i < 3; i++){
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
    
    return float4(input.sightRay, 1.0f);
}