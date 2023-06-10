struct InputVertex {
    float2 pos : POSITION;
    float3 color : COLOR;
};

struct OutputVertex {
    float4 pos : SV_Position;
    float3 color : COLOR;
};

OutputVertex main(InputVertex input)
{
    OutputVertex output;
    output.pos = float4(input.pos, 0.0f, 1.0f);
    output.color = input.color;
    return output;
}