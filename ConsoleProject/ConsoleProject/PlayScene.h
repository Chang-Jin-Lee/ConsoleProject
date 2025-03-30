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

	void DescriptionPlaySceneInitialize();
	void DescriptionPlaySceneLoadData();
	void DescriptionPlaySceneUpdate();
	void DescriptionPlaySceneInput();
	void DescriptionPlaySceneRender();

	void ShootingPlaySceneInitialize();
	void ShootingPlaySceneLoadData();
	void ShootingPlaySceneUpdate();
	void ShootingPlaySceneInput();
	void ShootingPlaySceneRender();
}