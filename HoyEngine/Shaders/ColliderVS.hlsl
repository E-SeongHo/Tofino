
#include "Common.hlsli"

cbuffer ColliderConstBuffer : register(b0)
{
    matrix world;
    float4 color;
};

struct PSInput
{
    float4 pos : SV_POSITION; // Screen Space
};

PSInput main(BasicVertexInput input)
{
    PSInput output;

    float4 pos = float4(input.pos, 1.0f);
    pos = mul(pos, world);
    pos = mul(pos, view);
    pos = mul(pos, projection);

    output.pos = pos;

    return output;
}