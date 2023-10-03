
cbuffer GlobalConstants : register(b1)
{
    matrix view;
    matrix projection;
};

struct BasicVertexInput
{
	float3 pos : POSITION;
	float3 color : COLOR;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};