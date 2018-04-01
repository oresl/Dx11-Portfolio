cbuffer Camera : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
};

struct VertexIn
{
	float4 Pos	: POSITION;
	float2 UV	: TEXCOORD;
	float3 Nor	: NORMAL;
};

struct PixelIn
{
	float4 Pos	: SV_POSITION;
	float2 UV	: TEXCOORD;
	float3 Nor	: NORMAL;
};

PixelIn VS(VertexIn input)
{
	PixelIn output;

	input.Pos.w = 1.0F;
	output.Pos = mul(input.Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);

	output.UV = input.UV;
	output.Nor = input.Nor;

	return output;
}

float4 PS(PixelIn input) : SV_Target
{
	return float4(1.0F, 1.0F, 1.0F, 1.0F);
}