#pragma once
#include <iostream>
#include <Windows.h>
#include "ConsoleRenderer.h"
#include "BaseDefine.h"

namespace PlayScene
{
	void Initialize();
	void LoadData();
	void ProcessInput();
	void Release();
	void Update();
	void Render();
	void BlinkSpeechNextButton();
	void SpeechContent();
	void CutScene();
}