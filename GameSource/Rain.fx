#include "RenderResource\\Pcf.fx"

struct VertexIn
{
	float4 Pos					: POSITION;
	row_major float4x4 World	: WORLD;
};

struct PixelIn
{
	float4 Pos					: SV_POSITION;
};

PixelIn VS(VertexIn input)
{
	PixelIn output;

	input.Pos.w = 1.0F;
	output.Pos = mul(input.Pos, input.World);
	output.Pos = mul(output.Pos, gCameraView);
	output.Pos = mul(output.Pos, gCameraProjection);

	return output;
}

float4 PS(PixelIn input) : SV_Target
{
	return float4(1.0F, 1.0F, 1.0F, 1.0F);
}