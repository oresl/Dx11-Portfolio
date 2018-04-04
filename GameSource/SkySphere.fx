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

	//����Ǵ� ���� ��
	float scatteredLight = abs(vertexGroundCos);

	//r ���� ����� ���� ���� �ʰ� �� ���� ������ ���´�.
	lightRGB.r = pow(lightVertexCos, 5); // * lightVertexCos * lightVertexCos * lightVertexCos * lightVertexCos;
	
	//�Ķ� ���� ��ǻ� ���� ����� ����
	//���� �Ķ� ���� ����ũ�� ����. ������ ����� �ҵ�
	//����Ǵ� ���� ������Ű�� ���� ������ ���� �Ķ����� �߰���Ŵ
	lightRGB.b = scatteredLight + lightGroundCos;
	lightRGB.g = (lightRGB.r + lightRGB.b) / 2;

	lightRGB = saturate(lightRGB);

	float4 result = float4(lightRGB, 1.0F); //lerp(_center, _apex, height);

	return result;
}

