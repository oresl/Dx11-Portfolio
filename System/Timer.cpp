#include "Timer.h"

Timer::Timer()
{
	mTickPerSecond		= 0;
	mTime				= 0;
	mPrevTime			= 0;
	mDeltaTime			= 0.0F;
	mPaused				= false;
}

Timer::~Timer()
{
}

void Timer::Initialize()
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&mTickPerSecond);
	assert(mTickPerSecond != 0);
}

void Timer::Update()
{
	if (mPaused)
	{
		mDeltaTime = 0.0F;
		return;
	}

	INT64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	mTime = currTime;

	// 매 루프마다 호출해 이전에 호출된 시간과의 차이를 계산해 프레임간의 시간을 확인
	mDeltaTime = (FLOAT)(mTime - mPrevTime) / (FLOAT)mTickPerSecond;
	mPrevTime = mTime;

	if (mDeltaTime < 0.0F)
		mDeltaTime = 0.0F;
}

void Timer::SetStart()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&mPrevTime);
	mPaused = false;
}

void Timer::SetPause()
{
	mPaused = true;
}

FLOAT Timer::GetDeltaTime() { return mDeltaTime; }