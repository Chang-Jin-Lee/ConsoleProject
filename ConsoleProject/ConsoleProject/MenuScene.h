#pragma once
#include <iostream>
#include <Windows.h>
#include "ConsoleRenderer.h"
#include "BaseDefine.h"

namespace MenuScene
{
	void Initialize();
	void ProcessInput();
	void Update();
	void Render();
	void LoadingBar(float value);
}