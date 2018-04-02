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