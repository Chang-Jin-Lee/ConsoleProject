#pragma once
#include "ConsoleRenderer.h"
#include "MenuScene.h"
#include "EndScene.h"
#include "PlayScene.h"
#include "BaseDefine.h"
#include "Time.h"
#include "FileController.h"
#include "Sound.h"

namespace Game
{
	void Initialize();
	void LoadData();
	void Update();
	void Render();
	void Release();
	ESceneState* GetCurrentSceneState();
	void ChangeScene(const ESceneState& scene);
	void GameExit();
}