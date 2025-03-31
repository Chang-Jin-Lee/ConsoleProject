#include "GameOverScene.h"
#include "Game.h"
#include "Input.h"
#include <fmod.hpp>

#pragma comment(lib, "fmod_vc.lib")

float m_fLastTimegameOverScene = 0;
float m_fGameOverScenecurrentTime = 0;
float m_fSceneChangeTimeOverScene = 10;

UI::FUI m_fenduiGameOverScene;

// Sound
FMOD::System* systemGameOverScene = nullptr;
FMOD::Sound* soundGameOverScene = nullptr;
FMOD::Channel* channelGameOverScene = nullptr;

void GameOverScene::Initialize()	// 게임 시작할 때 초기화
{
	m_fGameOverScenecurrentTime = Time::GetTotalTime();
	systemGameOverScene->playSound(soundGameOverScene, nullptr, false, &channelGameOverScene);
}

void GameOverScene::LoadData()
{
	if (FileController::FileRead("Images/EndingView/EndView.txt", "r", &m_fenduiGameOverScene.m_ppcontent, &m_fenduiGameOverScene.m_ippcontentSize))
	{
		m_fenduiGameOverScene.m_fAxis.X = 0;
		m_fenduiGameOverScene.m_fAxis.Y = 0;
	}

	// FMOD 시스템 초기화
	if (FMOD::System_Create(&systemGameOverScene) != FMOD_OK)
	{
		ConsoleRenderer::print((char*)"System_Create fail");
	}
	systemGameOverScene->init(512, FMOD_INIT_NORMAL, nullptr);
	if (systemGameOverScene->createSound("Music/bgm_GameOverScene.mp3", FMOD_DEFAULT, nullptr, &soundGameOverScene) != FMOD_OK) {
		ConsoleRenderer::print((char*)"createSound fail");
	}
}

void GameOverScene::Release()
{
	UI::Release(&m_fenduiGameOverScene);

	// 정리
	soundGameOverScene->release();
	systemGameOverScene->close();
	systemGameOverScene->release();
}

void GameOverScene::ProcessInput()
{
	if (Input::IsKeyPressed(VK_R))
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

void GameOverScene::Update()
{
	Input::Update();
	GameOverScene::ProcessInput();

	//m_fLastTimegameOverScene = Time::GetTotalTime() - m_fGameOverScenecurrentTime;
	//if (m_fLastTimegameOverScene >= m_fSceneChangeTimeOverScene)
	//{
	//	Game::ChangeScene(ESceneState::MENU);
	//}
}

void GameOverScene::Render()
{
	ConsoleRenderer::ScreenDrawUIFromFile(&m_fenduiGameOverScene, FG_WHITE);
}