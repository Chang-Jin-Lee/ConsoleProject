#include "EndScene.h"
#include "Game.h"
#include "Input.h"
#include <fmod.hpp>

#pragma comment(lib, "fmod_vc.lib")

float m_fLastTime = 0;
float m_fEndScenecurrentTime = 0;
float m_fSceneChangeTime = 10;

float m_fFPSTimeEndScene = 1 / 60;
//float m_fFPSTimeMenuScene = 60;
float m_fcountOneSecondEndScene = 0;

bool bNextSceneEndScene = false;

Object::FPlayerCharacter m_fIntroVideoEndScene;
// Sound
FMOD::System* systemEndScene = nullptr;
FMOD::Sound* soundEndScene = nullptr;
FMOD::Channel* channelEndScene = nullptr;

void EndScene::Initialize()	// 게임 시작할 때 초기화
{
	m_fEndScenecurrentTime = Time::GetTotalTime();

	systemEndScene->playSound(soundEndScene, nullptr, false, &channelEndScene);
}

void EndScene::LoadData()
{
	Object::SetPlayerAnimationName(&m_fIntroVideoEndScene, (char*)"EndVideo");
	Object::LoadAnimationData(&m_fIntroVideoEndScene);
	m_fIntroVideoEndScene.m_eAnimationState = Object::EAnimationState::FULLBODYIDLE;
	m_fIntroVideoEndScene.m_bPlayable = true;
	m_fIntroVideoEndScene.m_iColor = FG_WHITE;
	m_fIntroVideoEndScene.m_fAxis.X = 25;
	m_fIntroVideoEndScene.m_fAxis.Y = 50;
	m_fIntroVideoEndScene.m_iPlaybackCurrentSeconds = 0;

	// FMOD 시스템 초기화
	if (FMOD::System_Create(&systemEndScene) != FMOD_OK)
	{
		ConsoleRenderer::print((char*)"System_Create fail");
	}
	systemEndScene->init(512, FMOD_INIT_NORMAL, nullptr);
	if (systemEndScene->createSound("Music/bgm_EndScene.mp3", FMOD_DEFAULT, nullptr, &soundEndScene) != FMOD_OK) {
		ConsoleRenderer::print((char*)"createSound fail");
	}
}

void EndScene::Release()
{
	Object::Release(&m_fIntroVideoEndScene);
	// 정리
	soundEndScene->release();
	systemEndScene->close();
	systemEndScene->release();
}

void EndScene::ProcessInput()
{
	if (Input::IsKeyPressed(VK_R))
		if(bNextSceneEndScene)
			Game::ChangeScene(ESceneState::MENU);

	if (Input::IsKeyPressed(VK_ESCAPE)) //종료
		Game::GameExit();

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

	m_fLastTime += Time::GetElapsedTime();
	if (m_fLastTime >= m_fSceneChangeTime)
		bNextSceneEndScene = true;

	if (Time::GetTotalTime() - m_fcountOneSecondEndScene >= m_fFPSTimeEndScene)	// 0.5초에 한 번씩
	{
		if (m_fIntroVideoEndScene.m_bPlayable)
			m_fIntroVideoEndScene.m_iPlaybackCurrentSeconds = (m_fIntroVideoEndScene.m_iPlaybackCurrentSeconds + 1) % m_fIntroVideoEndScene.m_fanimation[m_fIntroVideoEndScene.m_eAnimationState].m_iMaxLength;
		m_fcountOneSecondEndScene = Time::GetTotalTime();
	}
}

void EndScene::Render()
{
	ConsoleRenderer::ScreenDrawPlayerWithAnimation(m_fIntroVideoEndScene.m_fAxis.X, m_fIntroVideoEndScene.m_fAxis.Y, &m_fIntroVideoEndScene.m_fanimation[m_fIntroVideoEndScene.m_eAnimationState].m_fui[m_fIntroVideoEndScene.m_iPlaybackCurrentSeconds], m_fIntroVideoEndScene.m_iColor);
}