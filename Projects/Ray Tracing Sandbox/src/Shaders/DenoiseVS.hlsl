struct VSOutput {
    float4 position : SV_Position;
};

VSOutput main(float2 corner : CORNER)
{
    VSOutput output;
    output.position = float4(corner, 0.0f, 1.0f);
    return output;
}