void Rotate(in out float3 p, float2 a) {
    [unroll(2)] for (uint i = 0; i < 2; i++) {
        const float c = cos(a[i]);
        const float s = sin(a[i]);
        float x = p.x * c - p[i + 1] * s;
        float y = p[i + 1] * c + p.x * s;
        p.x = x;
        p[i + 1] = y;
    }
}

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

VSOutput main(float2 corner : CORNER)
{
    VSOutput output;
    output.position = float4(corner, 0.0f, 1.0f);
    
    output.sightRay[0] = 1.0f / tan(camera[0].fov * 0.5f);
    output.sightRay.y = corner.y * camera[0].aspectRatio;
    output.sightRay.z = corner.x;
    Rotate(output.sightRay, camera[0].rotation);
    
    return output;
}