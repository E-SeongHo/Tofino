
TextureCube g_texture : register(t0); // Type ¡÷¿«!!
SamplerState g_sampler : register(s0);

struct EnvMapPSInput
{
	float4 pos : SV_POSITION;
	float2 normal : NORMAL; // for env mapping
	float2 uv : TEXCOORD;
};

float4 main(EnvMapPSInput input) : SV_TARGET
{
	float3 color = g_texture.Sample(g_sampler, input.pos.xyz).rgb;
	return float4(color, 1.0f);
}