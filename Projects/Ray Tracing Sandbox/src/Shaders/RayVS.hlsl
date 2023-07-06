#include "Ray_math.hlsli"

VSOutput main(float2 corner : CORNER)
{
    VSOutput output;
    output.position = float4(corner, 0.0f, 1.0f);

    float aspectRatio = (float)camera[0].windowSize.y / (float)camera[0].windowSize.x;
    output.sightRay[0] = 1.0f / tan(camera[0].fov * 0.5f);
    output.sightRay.y = corner.y * aspectRatio;
    output.sightRay.z = corner.x;
    Rotate(output.sightRay, camera[0].rotation);
    
    return output;
}