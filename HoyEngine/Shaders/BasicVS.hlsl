
#include "Common.hlsli"

Texture2D heightTexture : register(t0);
SamplerState g_sampler : register(s0);
SamplerState g_clampSampler : register(s1);

cbuffer ModelConstBuffer : register(b0)
{
	matrix world;
	matrix worldIT;
	int activeAlbedoMap;
	int activeNormalMap;
	int activeHeightMap;
	int padding;
};

cbuffer MaterialStatus : register(b1)
{
    float4 baseColor;
    float roughness;
    float metallic;
    float2 padding2;
	
    int hasAlbedoMap;
    int hasNormalMap;
    int hasHeightMap;
    int padding3;
}

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
	
	// normal
	float4 Nmodel = float4(input.normal, 0.0f);
	float3 Nworld = mul(Nmodel, worldIT).xyz;
	Nworld = normalize(Nworld);

	// tangent
	float4 Tmodel = float4(input.tangent, 0.0f);
	float3 Tworld = mul(Tmodel, world).xyz;
	Tworld = normalize(Tworld);

	// vertex position
	float4 pos = float4(input.pos, 1.0f);
	pos = mul(pos, world);

	// height mapping (in world space)
	int heightMapping = hasHeightMap && activeHeightMap ? 1 : 0;
	if (heightMapping)
	{
		float height = heightTexture.SampleLevel(g_sampler, input.uv, 1).r;
		height = 2.0f * height - 1.0f;
		pos.xyz += height * Nworld * 0.05f;
	}

	pos = mul(pos, view);
	pos = mul(pos, projection);

	output.pos = pos;
	output.color = input.color;
	output.normal = Nworld;
	output.uv = input.uv;
	output.tangent = Tworld;

	return output;
}