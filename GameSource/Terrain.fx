#include "RenderResource\\Pcf.fx"

Texture2D mHeightmap			: register(t0);
Texture2D mBlendmap				: register(t1);
Texture2DArray mLayermapArray	: register(t2);

cbuffer BufferData				: register(b11)
{
	// HS
	float MinCamDist;
	float MaxCamDist;
	float MinTessStep;
	float MaxTessStep;

	// DS
	float4 ClipPlane;

	// PS
	float TexelCellU;
	float TexelCellV;
	float CellSpacing;
	float2 Pad;
};

struct VertexIn
{
	float3 Pos : POSITION;
	float2 Tex : TEXCOORD;
};

struct HullIn
{
	float3 Pos : POSITION;
	float2 Tex : TEXCOORD;
};

HullIn VS(VertexIn input)
{
	HullIn output;

	output.Pos		= input.Pos;
	output.Pos.y	= mHeightmap.SampleLevel(gSSClamp, input.Tex, 0).r;
	output.Tex		= input.Tex;

	return output;
}

float CalcTessFactor(float3 p)
{
	float dist = distance(p, gCameraPosition);
	float satu = saturate((dist - MinCamDist) / (MaxCamDist - MinCamDist));

	return pow(2, (lerp(MaxTessStep, MinTessStep, satu)));
}

struct PatchTess
{
	float EdgeTess[4]   : SV_TessFactor;
	float InsideTess[2] : SV_InsideTessFactor;
};

PatchTess ConstantHS(InputPatch<HullIn, 4> patch, uint patchID : SV_PrimitiveID)
{
	PatchTess pt;

	float3 e0 = 0.5f	* (patch[0].Pos + patch[2].Pos);
	float3 e1 = 0.5f	* (patch[0].Pos + patch[1].Pos);
	float3 e2 = 0.5f	* (patch[1].Pos + patch[3].Pos);
	float3 e3 = 0.5f	* (patch[2].Pos + patch[3].Pos);
	float3  c = 0.25f	* (patch[0].Pos + patch[1].Pos + patch[2].Pos + patch[3].Pos);

	pt.EdgeTess[0] = CalcTessFactor(e0);
	pt.EdgeTess[1] = CalcTessFactor(e1);
	pt.EdgeTess[2] = CalcTessFactor(e2);
	pt.EdgeTess[3] = CalcTessFactor(e3);

	pt.InsideTess[0] = CalcTessFactor(c);
	pt.InsideTess[1] = pt.InsideTess[0];

	return pt;
}

struct DomainIn
{
	float3 Pos : POSITION;
	float2 Tex : TEXCOORD;
};

[domain("quad")]
[partitioning("fractional_even")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("ConstantHS")]
[maxtessfactor(64.0f)]
DomainIn HS(InputPatch<HullIn, 4> input,
	uint i : SV_OutputControlPointID,
	uint patchId : SV_PrimitiveID)
{
	DomainIn output;

	output.Pos = input[i].Pos;
	output.Tex = input[i].Tex;

	return output;
}

struct PixelIn
{
	float4 Pos		: SV_POSITION;
	float3 PosWorld : POSITION;
	float2 Tex		: TEXCOORD0;
	float2 TiledTex	: TEXCOORD1;
	//float4 ClipPlane	: SV_ClipDistance;
};

[domain("quad")]
PixelIn DS(PatchTess patchTess,
	float2 uv : SV_DomainLocation,
	const OutputPatch<DomainIn, 4> quad)
{
	PixelIn dout;

	// Bilinear interpolation.
	dout.PosWorld = lerp(
		lerp(quad[0].Pos, quad[1].Pos, uv.x),
		lerp(quad[2].Pos, quad[3].Pos, uv.x),
		uv.y);

	dout.Tex = lerp(
		lerp(quad[0].Tex, quad[1].Tex, uv.x),
		lerp(quad[2].Tex, quad[3].Tex, uv.x),
		uv.y);

	dout.PosWorld.y = mHeightmap.SampleLevel(gSSClamp, dout.Tex, 0).r;
	dout.TiledTex = dout.Tex * 50.0F;

	dout.Pos = mul(float4(dout.PosWorld, 1.0f), gCameraView);
	dout.Pos = mul(dout.Pos, gCameraProjection);

	//dout.ClipPlane = dot(mul(dout.Position, _World), _ClipPlane);

	return dout;
}

float4 PS(PixelIn pin) : SV_Target
{
	//return float4(1.0F, 1.0F, 1.0F, 1.0F);

	// ³ë¸», ÅºÁ¨Æ®

	//// ÀÎÁ¢ÇÑ quadÀÇ tex°ª
	//float2 leftTex		= pin.Tex + float2(-TexelCellU, 0.0f);
	//float2 rightTex		= pin.Tex + float2(TexelCellU, 0.0f);
	//float2 bottomTex	= pin.Tex + float2(0.0f,  TexelCellV);
	//float2 topTex		= pin.Tex + float2(0.0f, -TexelCellV);

	//// ÀÎÁ¢ÇÑ quadÀÇ y°ª
	//float leftY			= mHeightmap.SampleLevel(gSSClamp, leftTex, 0).r;
	//float rightY		= mHeightmap.SampleLevel(gSSClamp, rightTex, 0).r;
	//float bottomY		= mHeightmap.SampleLevel(gSSClamp, bottomTex, 0).r;
	//float topY			= mHeightmap.SampleLevel(gSSClamp, topTex, 0).r;

	//float3 tangent = float3(2.0F * CellSpacing, rightY - leftY, 0.0f);
	//tangent = normalize(tangent);

	//float3 bitan = float3(0.0F, bottomY - topY, -2.0f * CellSpacing);
	//bitan = normalize(bitan);

	//float3 normal = cross(tangent, bitan);

	// Texture
	float4 c0 = mLayermapArray.Sample(gSSWrap, float3(pin.TiledTex, 0.0f));	// darkdirt
	float4 c1 = mLayermapArray.Sample(gSSWrap, float3(pin.TiledTex, 1.0f));	// stone
	float4 c2 = mLayermapArray.Sample(gSSWrap, float3(pin.TiledTex, 2.0f));	// grass
	float4 c3 = mLayermapArray.Sample(gSSWrap, float3(pin.TiledTex, 3.0f));	// snow

	float4 t = mBlendmap.Sample(gSSWrap, pin.Tex);

	float4 texColor = c0;					// darkdirt
	texColor = lerp(texColor, c1, t.r);		// stone
	texColor = lerp(texColor, c2, t.g);		// grass
	texColor = lerp(texColor, c3, t.b);		// snow

	//// ºû
	//float4 ligColor		= gAmbient0;
	//float3 ligDir		= -gLightDirection;
	//float  ligAmount	= saturate(dot(normal, ligDir));

	//if (ligAmount > 0.0F)
	//	ligColor += (gDiffuse0 * ligAmount);

	//ligColor = saturate(ligColor * texColor);

	return texColor;
}