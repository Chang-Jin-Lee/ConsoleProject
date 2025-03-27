#pragma once
#include <iostream>
#include <Windows.h>
#include "ConsoleRenderer.h"
#include "BaseDefine.h"

namespace EndScene
{
	void Initialize();
	void LoadData();
	void Release();
	void ProcessInput();
	void Update();
	void Render();
}