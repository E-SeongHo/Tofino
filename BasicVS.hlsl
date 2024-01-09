
#include "Common.hlsli"

cbuffer ModelConstBuffer : register(b0)
{
	matrix world;
	matrix worldIT;
	Material material;
};

struct PSInput
{
	float4 pos : SV_POSITION;
	float3 color : COLOR;
	float3 normal : NORMAL; // World Space
	float2 uv   : TEXCOORD;
	float3 tangent : TANGENT; // World Space
};

PSInput main( BasicVertexInput input )
{
	PSInput output;

	float4 pos = float4(input.pos, 1.0f);
	pos = mul(pos, world);
	pos = mul(pos, view);
	pos = mul(pos, projection);

	float4 Nmodel = float4(input.normal, 0.0f);
	float3 Nworld = mul(Nmodel, worldIT).xyz;
	Nworld = normalize(Nworld);

	float4 Tmodel = float4(input.tangent, 0.0f);
	float3 Tworld = mul(Tmodel, world).xyz;
	Tworld = normalize(Tworld);

	output.pos = pos;
	output.color = input.color;
	output.normal = Nworld;
	output.uv = input.uv;
	output.tangent = Tworld;

	return output;
}