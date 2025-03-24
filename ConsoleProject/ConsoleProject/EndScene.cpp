#include "EndScene.h"
#include "Game.h"
#include "Input.h"

float m_fEndScenecurrentTime = 0;
float m_fSceneChangeTime = 0;

void EndScene::Initialize()	// 게임 시작할 때 초기화
{
	if (Game::GetTimer())
		m_fEndScenecurrentTime = Game::GetTimer()->GetTotalTime();
}

void EndScene::ProcessInput()
{
	if (Input::IsKeyPressed(VK_Q))
	{
		Game::ChangeScene(ESceneState::MENU);
	}

	if (Input::IsKeyPressed(VK_ESCAPE)) { //종료
		Game::GameExit();
	}

}

void EndScene::Update()
{
	Input::Update();
	EndScene::ProcessInput();
	if (Game::GetTimer())
	{
		float LastTime = Game::GetTimer()->GetTotalTime() - m_fEndScenecurrentTime;
		if (LastTime >= m_fSceneChangeTime)
		{
			Game::ChangeScene(ESceneState::MENU);
		}
	}
}

void EndScene::Render()
{
	ConsoleRenderer::ScreenDrawString(0, 0, "EndScene", FG_SKY_DARK);
	if (Game::GetTimer())
	{
		char array[20];
		sprintf_s(array, u8"%f", Game::GetTimer()->GetTotalTime() - m_fEndScenecurrentTime);
		ConsoleRenderer::ScreenDrawString(2, 2, "경과 시간 : ", FG_SKY_DARK);
		ConsoleRenderer::ScreenDrawString(3, 3, array, FG_SKY_DARK);
	}
}