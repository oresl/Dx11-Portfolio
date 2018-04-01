#pragma once
#include "..\Common\Pch.h"

class Timer
{
public:
	Timer();
	~Timer();
	void	Initialize();
	void	Update();
	void	SetPause();
	void	SetStart();
	FLOAT	GetDeltaTime();

private:
	INT64	mTickPerSecond;
	INT64	mTime;
	INT64	mPrevTime;
	FLOAT	mDeltaTime;
	bool	mPaused;

};