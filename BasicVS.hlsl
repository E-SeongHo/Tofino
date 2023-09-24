cbuffer WVP : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
};


struct VSInput
{
	float3 pos : POSITION;
	float3 color : COLOR;
};

struct PSInput
{
	float4 pos : SV_POSITION;
	float3 color : COLOR;
};

PSInput main( VSInput input )
{
	PSInput output;

	float4 pos = float4(input.pos, 1.0f);
	pos = mul(pos, world);
	pos = mul(pos, view);
	pos = mul(pos, projection);

	output.pos = pos;
	output.color = input.color;

	return output;
}