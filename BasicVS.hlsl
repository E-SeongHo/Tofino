
#include "Common.hlsli"

cbuffer ModelConstBuffer : register(b0)
{
	matrix world;
	matrix worldIT;
};

struct PSInput
{
	float4 pos : SV_POSITION;
	float3 color : COLOR;
	float2 uv   : TEXCOORD;
};

PSInput main( BasicVertexInput input )
{
	PSInput output;

	float4 pos = float4(input.pos, 1.0f);
	pos = mul(pos, world);
	pos = mul(pos, view);
	pos = mul(pos, projection);

	output.pos = pos;
	output.color = input.color;
	output.uv = input.uv;

	return output;
}