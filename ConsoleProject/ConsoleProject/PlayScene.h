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

	void OptionPlaySceneInitialize();
	void OptionPlaySceneLoadData();
	void OptionPlaySceneUpdate();
	void OptionPlaySceneInput();
	void OptionPlaySceneRender();

	void ShootingPlaySceneInitialize();
	void ShootingPlaySceneLoadData();
	void ShootingPlaySceneUpdate();
	void ShootingPlaySceneInput();
	void ShootingPlaySceneRender();

	int GetRandomInt(int min, int max);
	float GetRandomFloat(float min, float max);
}