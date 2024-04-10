
#include "Common.hlsli"

//cbuffer ModelConstants : register(b0)
//{
//	matrix world;
//}

struct EnvMapPSInput
{
	float4 pos : SV_POSITION;
	float4 posWorld : POSITION;
	// float3 normal : NORMAL; // for env mapping
	float2 uv : TEXCOORD;
};


EnvMapPSInput main(BasicVertexInput input)
{
	EnvMapPSInput output;

	float4 pos = float4(input.pos, 1.0f);

    //pos = mul(pos, world);
	output.posWorld = pos;
	pos = mul(pos, view);
	pos = mul(pos, projection);

	output.pos = pos;
	output.uv = input.uv;

	return output;
}
