#include "PlayScene.h"
#include "Input.h"
#include "Game.h"


void PlayScene::Initialize()	// ���� ������ �� �ʱ�ȭ
{
	
}

void PlayScene::ProcessInput()
{
	if (Input::IsKeyPressed(VK_Q))
	{
		Game::ChangeScene(ESceneState::END);
	}

	if (Input::IsKeyPressed(VK_ESCAPE)) { //����
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