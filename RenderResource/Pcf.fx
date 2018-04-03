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
// Filter ������ (���, Ȯ��, �Ӹʿ� ����)
MIN_MAG_MIP_LINEAR				:���, Ȯ��, �Ӹ� ��� ����
MIN_LINEAR_MAG_MIP_POINT		:���(����) / Ȯ��, �Ӹ�(����)
MIN_POINT_MAG_LINEAR_MIP_POINT	:���(����) / Ȯ��(����) / �Ӹ�(����)
ANISOTROPIC						:�� ���͸�(MaxAnisotropy = 4; �����ؾ� ��)

// Address ������
WRAP	:��ȯ
BORDER	:�׵θ� �� (BorderColor = float4(0.0F, 0.0F, 1.0F, 1.0F); �����ؾ� ��)
CLAMP	:����
MIRROR	:�ſ�
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