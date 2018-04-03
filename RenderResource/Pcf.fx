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
*/

SamplerState gSampler0
{
	Filter = MIN_MAG_MIP_LINEAR;
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