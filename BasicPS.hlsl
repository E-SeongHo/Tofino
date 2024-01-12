
#include "Common.hlsli"

Texture2D diffuseTexture : register(t0); 
Texture2D normalTexture : register(t1);

TextureCube envIrradianceTexture : register(t10);
TextureCube envSpecularTexture : register(t11);
Texture2D envBrdfLookUpTexture : register(t12);

SamplerState g_sampler : register(s0);
SamplerState g_clampSampler : register(s1);

cbuffer ModelConstBuffer : register(b0)
{
	matrix world;
	matrix worldIT;
	Material material;
	int activeAlbedoMap;
	int activeNormalMap;
	int activeHeightMap;
	int padding;
};

cbuffer MappingInfoBuffer : register(b1)
{
	int hasAlbedoMap;
	int hasNormalMap;
	int hasHeightMap;
	int padding2;
}

struct PSInput
{
	float4 pos : SV_POSITION; // World Space
	float3 color : COLOR;
	float3 normal : NORMAL; // World Space
	float2 uv : TEXCOORD;
	float3 tangent : TANGENT; // World Space
};

float3 TBNTransform(PSInput input)
{
	float3 N = input.normal;
	float3 T = normalize(input.tangent - dot(input.tangent, N) * N); // orthogonalization
	float3 B = cross(N, T);

	float3x3 TBN = float3x3(T, B, N);

	float3 normal = normalTexture.Sample(g_sampler, input.uv).rgb;
	normal = 2.0 * normal - 1.0;

	return normalize(mul(normal, TBN));
}

float4 main(PSInput input) : SV_TARGET
{
	float4 albedo = hasAlbedoMap && activeAlbedoMap ? diffuseTexture.Sample(g_sampler, input.uv) 
		: float4(material.albedo, 1.0f);

	float3 N = hasNormalMap && activeNormalMap ? TBNTransform(input) : input.normal;
	float3 L = normalize(-light.direction); // directional light
	float3 V = normalize(eye - input.pos); // vector to eye from pixel
	
	float NdotL = max(0.0f, dot(N, L));

	float3 H = normalize(V + L); // halfway vector of View and Light 
	float HdotN = max(0.0f, dot(H, N));

	// direct lighting
	float lighting = HdotN * light.strength; 

	float4 diffuse = material.roughness;
	float4 specular = material.metallic * pow(HdotN, light.coefficient);

	float4 color = albedo + float4((diffuse + specular) * lighting);

	// ambient lighting (IBL)
	float3 irradiance = envIrradianceTexture.Sample(g_sampler, input.normal).rgb;
	float3 diffuseIBL = material.roughness * irradiance;

	float costheta = dot(input.normal, V);
	float2 brdf = envBrdfLookUpTexture.Sample(g_clampSampler, float2(costheta, 1.0f - material.roughness)); // 1 - roughness for IBL baker
	float3 sampled = envSpecularTexture.Sample(g_sampler, input.normal).rgb;

	float3 specularIBL = material.metallic * (brdf.x + brdf.y) * sampled;

	float3 ambientLighting = diffuseIBL + specularIBL;

	color.rgb += ambientLighting;
	//color = clamp(color, 0.0f, 1000.0f);

	return color;
}