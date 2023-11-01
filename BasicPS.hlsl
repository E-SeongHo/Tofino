
Texture2D diffuseTexture : register(t0); 
SamplerState g_sampler : register(s0);

struct PSInput
{
	float4 pos : SV_POSITION;
	float3 color : COLOR;
	float2 uv : TEXCOORD;
};

float4 main(PSInput input) : SV_TARGET
{
	float4 sampleColor = diffuseTexture.Sample(g_sampler, input.uv);
	return sampleColor;
}