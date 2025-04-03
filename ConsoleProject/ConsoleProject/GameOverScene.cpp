#include "GameOverScene.h"
#include "Game.h"
#include "Input.h"

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
	Sound::PlaySoundWithVolume(systemGameOverScene, soundGameOverScene, channelGameOverScene, 1.0f);
}

void GameOverScene::LoadData()
{
	if (FileController::FileRead("Images/EndingView/EndView.txt", "r", &m_fenduiGameOverScene.m_ppcontent, &m_fenduiGameOverScene.m_ippcontentSize))
	{
		m_fenduiGameOverScene.m_fAxis.X = 0;
		m_fenduiGameOverScene.m_fAxis.Y = 0;
	}

	// FMOD 시스템 초기화
	Sound::Initialize(systemGameOverScene, 32, FMOD_INIT_NORMAL);
	Sound::LoadSound(systemGameOverScene, (char*)"Music", (char*)"bgm_EndScene.mp3", FMOD_DEFAULT, soundGameOverScene);
	Sound::SetSoundMod(soundGameOverScene, FMOD_LOOP_NORMAL);
}

void GameOverScene::Release()
{
	UI::Release(&m_fenduiGameOverScene);

	// FMOD 시스템 정리
	Sound::ReleaseChannel(channelGameOverScene);
	Sound::ReleaseSound(soundGameOverScene);
	Sound::ReleaseSystem(systemGameOverScene);
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
}

void GameOverScene::Render()
{
	ConsoleRenderer::ScreenDrawUIFromFile(&m_fenduiGameOverScene, FG_WHITE);
}