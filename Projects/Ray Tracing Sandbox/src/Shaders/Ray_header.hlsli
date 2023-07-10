#define MAX_LOCATION_SIZE 24
static const uint noInd = -1;
static const float lim = 1.0f / 256.0f;

struct VSOutput {
    float4 position : SV_Position;
    float3 sightRay : SIGHT_RAY;
};

struct Camera {
    float3 position;
    float2 rotation;
    float fov;
    uint2 windowSize;
};
StructuredBuffer<Camera> camera : register(t0);

struct Link {
    uint3 low, high;
};

struct Container {
    float3 offset;
    uint node;
    Link siblingLink;
};
StructuredBuffer<Container> containers : register(t1);

struct Box {
    float3 low;
    float3 high;
};

#define NodeType int
#define NODE_TYPE_ROOT 0
#define NODE_TYPE_HELPER 1
#define NODE_TYPE_OBJECT 2

struct Node {
    Box box;
    Link childLink;
    NodeType type;
};
StructuredBuffer<Node> nodes : register(t2);

struct Material {
    float3 emission;
    float3 reflection;
};
StructuredBuffer<Material> materials : register(t3);

struct PixelInfo {
    uint ind;
    float3 pos;
    float3 normal;
    float3 incomingLight;
};
RWStructuredBuffer<PixelInfo> pixelInfo : register(u1);
static uint pInd = 0;