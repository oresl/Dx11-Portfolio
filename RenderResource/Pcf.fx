cbuffer WorldBuffer		: register(b0)
{
	matrix gWorld;
};

cbuffer CameraBuffer	: register(b1)
{
	matrix gView;
	matrix gProjection;
	matrix gOrtho;
	matrix gReflection;
};

cbuffer LightBuffer		: register(b2)
{
	matrix gLightView;
	matrix gLightProjection;
	float3 gLightDirection;
	float3 gLightPosition;
	float2 gLightPad;
};

// Material
cbuffer MaterialBuffer0 : register(b3)
{
	float4 gDiffuse0;
	float4 gAmbient0;
	float4 gSpecular0;
	float  gSpecularPower0;
	float3 gMaterialPad0;
};

cbuffer MaterialBuffer1 : register(b4)
{
	float4 gDiffuse1;
	float4 gAmbient1;
	float4 gSpecular1;
	float  gSpecularPower1;
	float3 gMaterialPad1;
};

// Sampler State
/*
// Filter 설정값 (축소, 확대, 밉맵에 대한)
MIN_MAG_MIP_LINEAR				:축소, 확대, 밉맵 모두 선형
MIN_LINEAR_MAG_MIP_POINT		:축소(선형) / 확대, 밉맵(점형)
MIN_POINT_MAG_LINEAR_MIP_POINT	:축소(점형) / 확대(선형) / 밉맵(점형)
ANISOTROPIC						:비등성 필터링(MaxAnisotropy = 4; 지정해야 함)

// Address 설정값
WRAP	:순환
BORDER	:테두리 색 (BorderColor = float4(0.0F, 0.0F, 1.0F, 1.0F); 지정해야 함)
CLAMP	:한정
MIRROR	:거울

그외 기본값
** D3D11의 SamplerState 기본값
Filter				= D3D11_FILTER_MIN_MAG_MIP_LINEAR;
AddressU			= D3D11_TEXTURE_ADDRESS_CLAMP;
AddressV			= D3D11_TEXTURE_ADDRESS_CLAMP;
AddressW			= D3D11_TEXTURE_ADDRESS_CLAMP;
MipLODBias			= 0;
MaxAnisotropy		= 1;
ComparisonFunc		= D3D11_COMPARISON_NEVER;
BorderColor[ 0 ]	= 1.0f;
BorderColor[ 1 ]	= 1.0f;
BorderColor[ 2 ]	= 1.0f;
BorderColor[ 3 ]	= 1.0f;
MinLOD				= -3.402823466e+38F; // -FLT_MAX
MaxLOD				= 3.402823466e+38F; // FLT_MAX
*/

SamplerState gSampler0
{
	Filter			= MIN_MAG_MIP_LINEAR;
	
	AddressU		= WRAP;
	AddressV		= WRAP;
	AddressW		= WRAP;
};

SamplerState gSampler1
{
	Filter = MIN_LINEAR_MAG_MIP_POINT;
};

SamplerState gSampler2
{
	Filter = MIN_POINT_MAG_LINEAR_MIP_POINT;
};

SamplerState gSampler3
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;
};