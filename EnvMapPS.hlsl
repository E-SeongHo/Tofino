
TextureCube g_texture : register(t0); // Type ¡÷¿«!!
SamplerState g_sampler : register(s0);

struct EnvMapPSInput
{
	float4 pos : SV_POSITION;
	float3 posWorld : POSITION;
	// float3 normal : NORMAL; // for env mapping
	float2 uv : TEXCOORD;
};

float4 main(EnvMapPSInput input) : SV_TARGET
{
	float4 sampledColor = g_texture.Sample(g_sampler, input.posWorld.xyz);
	return sampledColor;
}