
Texture2D g_texture : register(t0); // resolved SRV
SamplerState g_sampler : register(s0); // clamp sampler

struct ToneMappingPSInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

float3 GammaCorrection(float3 color)
{
    float3 invGamma = float3(1, 1, 1) / 2.2f;
    color = pow(color, invGamma);

    return color;
}

float4 main(ToneMappingPSInput input) : SV_TARGET
{
    float3 renderedColor = g_texture.Sample(g_sampler, input.uv).rgb;

    // Tone Mapping
    float3 mapped = GammaCorrection(renderedColor);
    //float3 mapped = renderedColor;
    //return float4(0.0f, 1.0f, 1.0f, 1.0f);
    return float4(mapped, 1.0f);
}