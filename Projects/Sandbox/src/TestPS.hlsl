struct OutputVertex {
    float4 pos : SV_Position;
    float3 color : COLOR;
};

float4 main(OutputVertex input) : SV_TARGET
{
	return float4(input.color, 1.0f);
}