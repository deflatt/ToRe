#include "Ray_header.hlsli"

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

static uint seed = 0;
float Rand(){
    if (seed == 0)
        seed = pInd;
    seed = seed * 747796405 + 2891336453;
    uint result = ((seed >> ((seed >> 28) + 4)) ^ seed) * 277803737;
    result = (result >> 22) ^ result;
    return result / 4294967295.0;
}

float RandNorm(){
    float theta = 2.0 * 3.1415926 * Rand();
    float rho = sqrt(-2.0f * log(Rand()));
    return rho * cos(theta);
}

float3 RandDir(){
    return normalize(float3(RandNorm(), RandNorm(), RandNorm()));
}

float3 RandSurfaceDir(float3 normal){
    float3 dir = RandDir();
    if (dot(dir, normal) < 0.0f)
        return -dir;
    else
        return dir;
}