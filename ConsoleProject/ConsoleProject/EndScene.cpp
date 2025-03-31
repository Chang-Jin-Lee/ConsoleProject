#include "EndScene.h"
#include "Game.h"
#include "Input.h"

float m_fLastTime = 0;
float m_fEndScenecurrentTime = 0;
float m_fSceneChangeTime = 10;

UI::FUI m_fenduiEndScene;

void EndScene::Initialize()	// 게임 시작할 때 초기화
{
	m_fEndScenecurrentTime = Time::GetTotalTime();
}

void EndScene::LoadData()
{
	if (FileController::FileRead("Images/EndingView/EndView.txt", "r", &m_fenduiEndScene.m_ppcontent, &m_fenduiEndScene.m_ippcontentSize))
	{
		m_fenduiEndScene.m_fAxis.X = 0;
		m_fenduiEndScene.m_fAxis.Y = 0;
	}
}

void EndScene::Release()
{
	UI::Release(&m_fenduiEndScene);
}

void EndScene::ProcessInput()
{
	if (Input::IsKeyPressed(VK_SPACE))
	{
		Game::ChangeScene(ESceneState::MENU);
	}

	if (Input::IsKeyPressed(VK_ESCAPE)) //종료
	{
		Game::GameExit();
	}

	if (Input::IsKeyPressed(VK_0))
	{
		int fontsize = ConsoleRenderer::GetScreenFontSize();
		ConsoleRenderer::SetScreenFontSize(fontsize + 1);
		ConsoleRenderer::ScreenInit();
	}

	if (Input::IsKeyPressed(VK_9))
	{
		int fontsize = ConsoleRenderer::GetScreenFontSize();
		ConsoleRenderer::SetScreenFontSize(fontsize - 1);
		ConsoleRenderer::ScreenInit();
	}
}

void EndScene::Update()
{
	Input::Update();
	EndScene::ProcessInput();

	m_fLastTime = Time::GetTotalTime() - m_fEndScenecurrentTime;
	if (m_fLastTime >= m_fSceneChangeTime)
	{
		Game::ChangeScene(ESceneState::MENU);
	}
}

void EndScene::Render()
{
	ConsoleRenderer::ScreenDrawUIFromFile(&m_fenduiEndScene, FG_WHITE);
}