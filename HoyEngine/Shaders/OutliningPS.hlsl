
cbuffer OutliningInfo : register(b0)
{   // world space
    float3 pos;
    float3 normal;
    float3 eye;
};

float rimPower = 1.0f;

float4 main() : SV_TARGET
{
    // Rim Lighting
    float3 toEye = normalize(eye - pos);


	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}
