StructuredBuffer<float> vals : register(t0);

float4 main(float4 pos : SV_Position) : SV_Target
{
    float x = pos.x / 1280.0f * 2.0f - 1.0f;
    float y = pos.y / 720.0f * 2.0f - 1.0f;
    x *= 4;
    y *= -4;
    
    float val = pow(2.7f, -(x * x));
    return (vals[0] * vals[0]) * 1.0f - abs(y - val);
}