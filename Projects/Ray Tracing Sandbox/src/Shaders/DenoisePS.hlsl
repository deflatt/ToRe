static const uint noInd = -1;
static const float lim = 1.0f / 256.0f;

struct VSOutput {
    float4 position : SV_Position;
};

struct Camera {
    float3 position;
    float2 rotation;
    float fov;
    uint2 windowSize;
};
StructuredBuffer<Camera> camera : register(t0);

struct Material {
    float3 emission;
    float3 reflection;
};
StructuredBuffer<Material> materials : register(t1);

struct PixelInfo {
    uint ind;
    float3 pos;
    float3 normal;
    float3 incomingLight;
};
RWStructuredBuffer<PixelInfo> pixelInfo : register(u1);

#define denoiseRad 6
#define minCosDev 0.95f
#define maxPosDev 0.01f

float4 main(VSOutput vsOutput) : SV_TARGET
{
    int2 pPos = (int2)vsOutput.position.xy;
    int2 wSize = camera[0].windowSize;
    uint pInd = pPos.y * wSize.x + pPos.x;
    
    PixelInfo inInfo = pixelInfo[pInd];
    if (inInfo.ind == noInd){
        return float4(inInfo.incomingLight, 1.0f);
    }
    uint side = 0;
    for (; side < 3; side++){
        if (inInfo.normal[side] != 0.0f)
            break;
    }
    
    uint numUsed = 0;
    float3 total = 0.0f;
    for (int y = max(0, pPos.y - denoiseRad); y <= min(wSize.y - 1, pPos.y + denoiseRad); y++){
        for (int x = max(0, pPos.x - denoiseRad); x <= min(wSize.x - 1, pPos.x + denoiseRad); x++){
            uint curInd = y * wSize.x + x;
            PixelInfo info = pixelInfo[curInd];
            
            float cosDev = dot(inInfo.normal, info.normal);
            if (cosDev < minCosDev)
                continue;
            if (abs(inInfo.pos[side] - info.pos[side]) > maxPosDev)
                continue;
            total += info.incomingLight;
            numUsed++;
        }
    }
    
    total /= (float)numUsed;
    Material mat = materials[inInfo.ind];
    total *= mat.reflection;
    total += mat.emission;
    
    return float4(total, 1.0f);
}