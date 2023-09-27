
struct CopyVSInput
{
    float3 position : POSITION;
    float3 color : COLOR;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

struct ToneMappingPSInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

ToneMappingPSInput main(CopyVSInput input)
{
    ToneMappingPSInput output;

    output.position = float4(input.position, 1.0);
    output.uv = input.uv;

    return output;
}
