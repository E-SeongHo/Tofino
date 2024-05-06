
#include "Common.hlsli"

// Not support for Instanced Object 
cbuffer ModelConstBuffer : register(b0)
{
    matrix world;
    matrix worldIT;
};

struct NormalGSInput
{
    float4 posWorld : SV_POSITION;
    float3 normalWorld : NORMAL;
};

NormalGSInput main(BasicVertexInput input)
{
    NormalGSInput output;
    
    float4 pos = float4(input.pos, 1.0f);
    pos = mul(pos, world);
    output.posWorld = pos;
    
    output.normalWorld = mul(input.normal, worldIT);
    normalize(output.normalWorld);

    return output;
}