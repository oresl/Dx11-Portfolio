#include "RenderResource\\Pcf.fx"

Texture2D mTrunk : register(t0);

struct VertexIn
{
	float4 Pos					: POSITION;
	float2 Tex					: TEXCOORD;
	row_major float4x4 World	: WORLD;
};

struct PixelIn
{
	float4 Pos	: SV_POSITION;
	float2 Tex	: TEXCOORD;
};

PixelIn VS(VertexIn input)
{
	PixelIn output;

	input.Pos.w = 1.0F;
	output.Pos = mul(input.Pos, input.World);
	output.Pos = mul(output.Pos, gCameraView);
	output.Pos = mul(output.Pos, gCameraProjection);

	output.Tex = input.Tex;

	return output;
}

float4 PS(PixelIn input) : SV_Target
{
	float4 color = mTrunk.Sample(gSSWrap, input.Tex);

	return color;
}