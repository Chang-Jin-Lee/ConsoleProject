#pragma once
#include "ConsoleRenderer.h"
#include "MenuScene.h"
#include "EndScene.h"
#include "PlayScene.h"
#include "BaseDefine.h"
#include "Time.h"

namespace Game
{
	void Initialize();
	void SceneInitialize();
	void Update();
	void Render();
	void Release();
	Time* GetTimer();
	ESceneState* GetCurrentSceneState();
	ESceneState* GetNextSceneState();
	void ChangeScene(const ESceneState& scene);
	void GameExit();
}