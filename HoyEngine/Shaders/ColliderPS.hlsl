
cbuffer ColliderConstBuffer : register(b0)
{
    matrix world;
    float4 color;
};

struct PSInput
{
    float4 pos : SV_POSITION; // Screen Space
};

float4 main(PSInput input) : SV_TARGET
{
    return color;
}