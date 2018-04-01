#pragma once
#include "..\Common\Pch.h"

class Game
{
public:
	Game();
	~Game();
	void Initialize();
	void Update(FLOAT delta);
	void PreRender();
	void Render();

private:

};

