struct OutputVertex {
    float4 pos : SV_Position;
    float3 color : COLOR;
};

RWStructuredBuffer<float3> buffer : register(u1);

float4 main(OutputVertex input) : SV_TARGET
{
    uint2 pixelPosition = input.pos.xy;
    if (pixelPosition.x == 640 && pixelPosition.y == 360)
        buffer[0].r += 0.001f;
	return float4(buffer[0], 1.0f);
}