#pragma once
#include <iostream>
#include <Windows.h>
#include "ConsoleRenderer.h"
#include "BaseDefine.h"

namespace PlayScene
{
	void Initialize();
	void ProcessInput();
	void Update();
	void Render();
}