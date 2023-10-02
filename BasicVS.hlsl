
#include "Common.hlsli"

cbuffer ModelConstBuffer : register(b0)
{
	matrix world;
};

struct PSInput
{
	float4 pos : SV_POSITION;
	float3 color : COLOR;
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

	return output;
}