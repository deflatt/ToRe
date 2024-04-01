
struct Camera {
    float3 pos;
    float2 rot;
};
StructuredBuffer<Camera> cameras : register(t0);

struct VSOutput {
    float4 pos : SV_Position;
    float3 sightRay : SIGHT_RAY;
};

VSOutput main(float2 pos : POSITION) {
    Camera cam = cameras[0];
    
    float aspectRatio = 720.0f / 1280.0f;
    float fov = 3.14f / 2.0f;
    
    float3 midRay = { sin(cam.rot.y) * cos(cam.rot.x), sin(cam.rot.y) * sin(cam.rot.x), cos(cam.rot.y) };
    float3 xv = { sin(cam.rot.y) * (-sin(cam.rot.x)), sin(cam.rot.y) * cos(cam.rot.x), 0 };
    float3 zv = { cos(cam.rot.y) * cos(cam.rot.x), cos(cam.rot.y) * sin(cam.rot.x), -sin(cam.rot.y) };
    xv = normalize(xv);
    zv = normalize(zv) * aspectRatio;
    
    // Scale midRay to change fov
    
    VSOutput output;
    output.pos = float4(pos, 1.0f, 1.0f);
    output.sightRay = midRay * (1.0f / tan(fov / 2.0f)) + xv * pos[0] + zv * (-pos[1]);
    return output;
}