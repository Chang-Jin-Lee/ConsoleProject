#pragma once
#include <iostream>
#include <Windows.h>
#include "ConsoleRenderer.h"
#include "BaseDefine.h"

namespace AnimationScene
{
	void Initialize();
	void LoadData();
	void ProcessInput();
	void Release();
	void Update();
	void Render();
}