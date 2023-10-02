
#include "Common.hlsli"

struct ToneMappingPSInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

ToneMappingPSInput main(BasicVertexInput input)
{
    ToneMappingPSInput output;

    output.position = float4(input.pos, 1.0);
    output.uv = input.uv;

    return output;
}
