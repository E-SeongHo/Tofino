
struct Light
{
	float3 pos;
	float strength;
	float3 direction;
	float coefficient;
};

struct Material
{
	float3 albedo;
	float roughness;
	float metallic;
	float3 padding;
};

cbuffer GlobalConstants : register(b1)
{
    matrix view;
    matrix projection;

	float3 eye; // World Space
	float padding1;
	Light light;
};

struct BasicVertexInput
{
	float3 pos : POSITION; // Model Space
	float3 color : COLOR; 
	float3 normal : NORMAL; // Model Space
	float2 uv : TEXCOORD;
	float3 tangent : TANGENT; // Model Space
};