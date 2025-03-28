#include "EndScene.h"
#include "Game.h"
#include "Input.h"

float m_fLastTime = 0;
float m_fEndScenecurrentTime = 0;
float m_fSceneChangeTime = 5;
const char* m_cgameEndingGuideEndScene = "Tank you To Play !";
const char* m_crestartEndScene = "Restart To Press Space Bar ";
const char* m_cexitEndScene = "Exit Game To Press ESC key ";
const char* m_cnextSceneGuideEndScene = "Back To Main Menu in 5 seconds... ";

void EndScene::Initialize()	// 게임 시작할 때 초기화
{
	m_fEndScenecurrentTime = Time::GetTotalTime();
}

void EndScene::LoadData()
{

}

void EndScene::Release()
{

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
	ConsoleRenderer::ScreenDrawString(ConsoleRenderer::ScreenCenter(m_cgameEndingGuideEndScene), int(ConsoleRenderer::ScreenHeight() * 0.2), m_cgameEndingGuideEndScene, FG_WHITE);
	ConsoleRenderer::ScreenDrawString(ConsoleRenderer::ScreenCenter(m_crestartEndScene), int(ConsoleRenderer::ScreenHeight() * 0.5), m_crestartEndScene, FG_WHITE);
	ConsoleRenderer::ScreenDrawString(ConsoleRenderer::ScreenCenter(m_cexitEndScene), int(ConsoleRenderer::ScreenHeight() * 0.6), m_cexitEndScene, FG_WHITE);

	char array[70];
	sprintf_s(array, "%.2f", m_fLastTime);
	ConsoleRenderer::ScreenDrawString(ConsoleRenderer::ScreenCenter(m_cnextSceneGuideEndScene), int(ConsoleRenderer::ScreenHeight() * 0.7), m_cnextSceneGuideEndScene, FG_WHITE);
	ConsoleRenderer::ScreenDrawString(ConsoleRenderer::ScreenCenter(array), int(ConsoleRenderer::ScreenHeight() * 0.8), array, FG_SKY_DARK);

}