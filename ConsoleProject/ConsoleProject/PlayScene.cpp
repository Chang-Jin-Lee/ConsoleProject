#include "PlayScene.h"
#include "Input.h"
#include "Game.h"


void PlayScene::Initialize()	// 게임 시작할 때 초기화
{
	
}

void PlayScene::ProcessInput()
{
	if (Input::IsKeyPressed(VK_Q))
	{
		Game::ChangeScene(ESceneState::END);
	}

	if (Input::IsKeyPressed(VK_ESCAPE)) { //종료
		Game::GameExit();
	}
}

void PlayScene::Update()
{
	Input::Update();
	PlayScene::ProcessInput();
}

void PlayScene::Render()
{
	ConsoleRenderer::ScreenDrawString(0, 0, u8"PlayScene", FG_GREEN_DARK);
}