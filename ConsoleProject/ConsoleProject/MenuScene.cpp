#include "MenuScene.h"
#include "Input.h"
#include "Game.h"
#include "UI.h"
#include <fmod.hpp>

#pragma comment(lib, "fmod_vc.lib")

// Time 관련
float m_fcurrentTime = 0;
float m_floadingTime = 5;
float m_fMenuLastTime = 0;

float m_finitialOneSecond = 0;
float m_fcountOneSeconds = 0;
float m_fPlayer_x = 0;
float m_fPlayer_y = 0;

float m_fFPSTimeMenuScene = 1 / 60;
//float m_fFPSTimeMenuScene = 60;
float m_fcountOneSecond = 0;
bool bcatBlink = false;

UI::FUI m_fTitleFile;
UI::FUI m_fCursor;
UI::FUI m_fStartButton;
UI::FUI m_fExitButton;

Object::FPlayerCharacter m_fIntroVideo;

// Sound
FMOD::System* systemMenuScene = nullptr;
FMOD::Sound* soundMenuScene = nullptr;
FMOD::Channel* channelMenuScene = nullptr;

void MenuScene::Initialize()	// 게임 시작할 때 초기화
{
	Object::SetPlayerAnimationName(&m_fIntroVideo, (char*)"RapiLobby");
	Object::LoadAnimationData(&m_fIntroVideo);
	m_fIntroVideo.m_eAnimationState = Object::EAnimationState::FULLBODYIDLE;
	m_fIntroVideo.m_bPlayable = true;
	m_fIntroVideo.m_iColor = FG_WHITE;
	m_fIntroVideo.m_fAxis.X = 0;
	m_fIntroVideo.m_fAxis.Y = 10;
	m_fIntroVideo.m_iPlaybackCurrentSeconds = 0;

	m_fMenuLastTime = Time::GetTotalTime();
	m_finitialOneSecond = Time::GetTotalTime();
	m_fcountOneSecond = Time::GetTotalTime();


	systemMenuScene->playSound(soundMenuScene, nullptr, false, &channelMenuScene);
}
void MenuScene::LoadData()
{
	//Images/CityForest01/CityForest_01.txt
	//Video/Aru/frame_0001.txt
	//Images/CityForest01/CityForest_01.txt
	//Images/Test.txt
	//Images/text/Dialogue/text_0008_12.txt
	if (FileController::FileRead("Images/text/text_shifty_10.txt", "r", &m_fTitleFile.m_ppcontent, &m_fTitleFile.m_ippcontentSize))
	{
		m_fTitleFile.m_fAxis.X = 0;
		m_fTitleFile.m_fAxis.Y = 0;
	}

	// FMOD 시스템 초기화
	if (FMOD::System_Create(&systemMenuScene) != FMOD_OK)
	{
		ConsoleRenderer::print((char*)"System_Create fail");
	}
	systemMenuScene->init(512, FMOD_INIT_NORMAL, nullptr);
	if (systemMenuScene->createSound("Music/bgm_MenuScene.mp3", FMOD_DEFAULT, nullptr, &soundMenuScene) != FMOD_OK) {
		ConsoleRenderer::print((char*)"createSound fail");
	}
}

void MenuScene::ProcessInput()
{
	if (Input::IsKeyPressed(VK_ESCAPE))  //종료
	{
		Game::GameExit();
	}

	if (Input::IsKeyPressed(VK_RETURN) || Input::IsKeyPressed(VK_SPACE))
	{
		Game::ChangeScene(ESceneState::ANIMATION);
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

void MenuScene::Release()
{
	UI::Release(&m_fTitleFile);
	Object::Release(&m_fIntroVideo);
	// 정리
	soundMenuScene->release();
	systemMenuScene->close();
	systemMenuScene->release();
}

void MenuScene::Update()
{
	Input::Update();
	MenuScene::ProcessInput();

	m_fMenuLastTime = Time::GetTotalTime() - m_fcurrentTime;
	if (m_fMenuLastTime >= m_floadingTime + 1)
	{
		//Game::ChangeScene(ESceneState::PLAY);
	}

	m_fcountOneSeconds = Time::GetTotalTime() - m_finitialOneSecond;
	if (m_fcountOneSeconds >= 1)
	{
		m_finitialOneSecond = Time::GetTotalTime();
		m_fPlayer_x += 3;
		m_fcountOneSeconds = 0;
	}

	if (Time::GetTotalTime() - m_fcountOneSecond >= m_fFPSTimeMenuScene)	// 0.5초에 한 번씩
	{
		if (m_fIntroVideo.m_bPlayable)
		{
			m_fIntroVideo.m_iPlaybackCurrentSeconds = (m_fIntroVideo.m_iPlaybackCurrentSeconds + 1) % m_fIntroVideo.m_fanimation[m_fIntroVideo.m_eAnimationState].m_iMaxLength;
		}
		m_fcountOneSecond = Time::GetTotalTime();
	}
}

void MenuScene::Render()
{
	//ConsoleRenderer::ScreenDrawUIFromFile(&m_fTitleFile, FG_WHITE);
	ConsoleRenderer::ScreenDrawPlayerWithAnimation(m_fIntroVideo.m_fAxis.X, m_fIntroVideo.m_fAxis.Y, &m_fIntroVideo.m_fanimation[m_fIntroVideo.m_eAnimationState].m_fui[m_fIntroVideo.m_iPlaybackCurrentSeconds], m_fIntroVideo.m_iColor);
}