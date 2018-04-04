#include "RenderResource\\Pcf.fx"

struct VertexIn
{
	float4 Pos	: POSITION;
};

struct PixelIn
{
	float4 Pos			: SV_POSITION;
	float4 PosWorld		: TEXCOORD0;
	float3 PosVertex	: TEXCOORD1;
};

PixelIn VS(VertexIn input)
{
	PixelIn output;

	input.Pos.w = 1.0F;

	output.PosWorld		= input.Pos;
	output.PosVertex	= input.Pos.xyz;

	output.Pos = mul(input.Pos, gWorld);
	output.Pos = mul(output.Pos, gCameraView);
	output.Pos = mul(output.Pos, gCameraProjection);

	return output;
}

float4 PS(PixelIn input) : SV_Target
{
	float  height			= saturate(input.PosWorld.y);
	float3 lightPos			= -gLightDirection;
	float3 vertexPos		= normalize(input.PosVertex);

	float lightVertexCos	= dot(lightPos, vertexPos);
	float vertexGroundCos	= dot(vertexPos, float3(0, 1, 0));
	float lightGroundCos	= dot(lightPos, float3(0, 1, 0));

	float3 lightRGB;

	//산란되는 빛의 양
	float scatteredLight = abs(vertexGroundCos);

	//r 빛은 산란이 거의 되지 않고 다 직접 광으로 들어온다.
	lightRGB.r = pow(lightVertexCos, 5); // * lightVertexCos * lightVertexCos * lightVertexCos * lightVertexCos;
	
	//파란 빛은 사실상 거의 산란된 빛들
	//지금 파란 빛은 페이크가 심함. 수정을 해줘야 할듯
	//산란되는 양을 고정시키고 빛의 각도에 따라 파란빛을 추가시킴
	lightRGB.b = scatteredLight + lightGroundCos;
	lightRGB.g = (lightRGB.r + lightRGB.b) / 2;

	lightRGB = saturate(lightRGB);

	float4 result = float4(lightRGB, 1.0F); //lerp(_center, _apex, height);

	return result;
}

