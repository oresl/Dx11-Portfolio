#include "RenderResource\\Pcf.fx"

Texture2D mCloud : register(t0);
Texture2D mNoise : register(t1);

cbuffer CustomBuffer : register(b11)
{
	float Translation;
	float Scale;
	float Brightness;
	float Pad;
};

struct VertexIn
{
	float4 Pos : POSITION;
	float2 Tex : TEXCOORD;
};

struct PixelIn
{
	float4 Pos			: SV_POSITION;
	float2 Tex			: TEXCOORD0;
	float3 PosVertex	: TEXCOORD1;
};

PixelIn VS(VertexIn input)
{
	PixelIn output;

	input.Pos.w = 1.0F;

	output.PosVertex = input.Pos.xyz;

	output.Pos = mul(input.Pos, gWorld);
	output.Pos = mul(output.Pos, gCameraView);
	output.Pos = mul(output.Pos, gCameraProjection);

	output.Tex = input.Tex;

	return output;
}

float4 PS(PixelIn input) : SV_Target
{
	float3 lightPos			= gLightDirection;
	float3 vertexPos		= normalize(input.PosVertex);
	float  lightVertexCos	= dot(lightPos, vertexPos);

	// 텍스쳐 이동
	input.Tex.x += Translation;

	// Scale과 이동값을 적용
	float4 noise;
	noise		= mNoise.Sample(gSampler0, input.Tex);
	noise		= noise * Scale;
	noise.xy	= noise.xy + input.Tex.xy + Translation;

	// 밝기 곱하기
	// 빛의 방향에 따라서 구름의 가시 결정
	float4 color;
	color = mCloud.Sample(gSampler0, noise.xy);
	color = color * Brightness * -(lightVertexCos - 0.3F);

	return color;
}