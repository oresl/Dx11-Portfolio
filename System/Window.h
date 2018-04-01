#pragma once
#include "..\Common\Pch.h"
#include "..\Config\Config.h"
#include "..\Graphic\Direct3D.h"
#include "..\GameSource\Game.h"
#include "DirectInput.h"
#include "Timer.h"

class Window
{
public:
	Window();
	~Window();
	void	Initialize();
	void	Loop();

private:
	Timer*	mTimer;
	Game*	mGame;

	void	InitializeConfig();
	void	InitializeWindow();
	void	InitializeDevices();

};

