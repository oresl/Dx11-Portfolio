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
	output.Pos = mul(output.Pos, gCameraView);
	output.Pos = mul(output.Pos, gCameraProjection);

	output.Tex = input.Tex;
	output.Nor = mul(input.Nor, (float3x3)gWorld);
	output.Nor = normalize(output.Nor);

	return output;
}

float4 PS(PixelIn input) : SV_Target
{
	// �ؽ���
	float4 texColor = mTexture.Sample(gSampler0, input.Tex);
	
	// ��
	float4 ligColor = gAmbient0;
	float3 ligDir	= -gLightDirection;
	float ligAmount = saturate(dot(input.Nor, ligDir));

	// TODO:: �ذ� ������ �ٴ�ǥ���� ��ó���� ��� �ؾ��ұ�
	// �Ʒ��� ���� ó���ϸ� ���ڱ� ���ϰ� ���� ����� ���ڿ�������
	if (ligAmount > 0.0F && ligDir.y > -0.1F)
		ligColor += gDiffuse0 * ligAmount;

	ligColor = saturate(ligColor);

	float4 color = ligColor * texColor;

	return color;
}