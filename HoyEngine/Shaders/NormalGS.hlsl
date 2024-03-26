
#include "Common.hlsli"

struct NormalGSInput
{
    float4 posWorld : SV_POSITION;
    float3 normalWorld : NORMAL;
};

struct NormalPSInput
{
    float4 pos : SV_POSITION;
    float3 color : COLOR;
};

static const float lineScale = 0.2;

[maxvertexcount(2)]
void main(point NormalGSInput input[1], uint primID: SV_PrimitiveID, 
    inout LineStream<NormalPSInput> outputStream)
{
    NormalPSInput output;

    float4 normal = float4(input[0].normalWorld, 0.0f);
    float4 pos = input[0].posWorld;
    
    output.pos = mul(pos, view);
    output.pos = mul(output.pos, projection);
    output.color = float3(1.0f, 0.0f, 0.0f);
    outputStream.Append(output);

    output.pos = mul(pos + normal * lineScale, view);
    output.pos = mul(output.pos, projection);
    output.color = float3(1.0f, 1.0f, 0.0f);
    outputStream.Append(output);
}