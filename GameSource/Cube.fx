#include "RenderResource\\Pcf.fx"

Texture2D mTexture : register(t0);

struct VertexIn
{
	float4 Pos	: POSITION;
	float2 Tex	: TEXCOORD;
	float3 Nor	: NORMAL;
};

struct PixelIn
{
	float4 Pos	: SV_POSITION;
	float2 Tex	: TEXCOORD;
	float3 Nor	: NORMAL;
};

PixelIn VS(VertexIn input)
{
	PixelIn output;

	input.Pos.w = 1.0F;
	output.Pos = mul(input.Pos, gWorld);
	output.Pos = mul(output.Pos, gView);
	output.Pos = mul(output.Pos, gProjection);

	output.Tex = input.Tex;
	output.Nor = input.Nor;

	return output;
}

float4 PS(PixelIn input) : SV_Target
{
	float4 color = mTexture.Sample(gSampler0, input.Tex);

	return color;
}