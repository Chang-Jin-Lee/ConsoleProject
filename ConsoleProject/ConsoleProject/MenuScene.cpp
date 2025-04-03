#include "MenuScene.h"
#include "Input.h"
#include "Game.h"
#include "UI.h"

// Time 관련
float m_fFPSTimeMenuScene = 1 / 60;
float m_fcountOneSecond = 0;
bool bcatBlink = false;

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

	m_fcountOneSecond = Time::GetTotalTime();

	Sound::PlaySoundWithVolume(systemMenuScene, soundMenuScene, channelMenuScene, 1.0f);
}
void MenuScene::LoadData()
{
	Sound::Initialize(systemMenuScene, 32, FMOD_INIT_NORMAL);
	Sound::LoadSound(systemMenuScene, (char*)"Music", (char*)"bgm_MenuScene.mp3", FMOD_DEFAULT, soundMenuScene);
	Sound::SetSoundMod(soundMenuScene, FMOD_LOOP_NORMAL);
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
	Object::Release(&m_fIntroVideo);
	// FMOD 시스템 정리
	Sound::ReleaseChannel(channelMenuScene);
	Sound::ReleaseSound(soundMenuScene);
	Sound::ReleaseSystem(systemMenuScene);
}

void MenuScene::Update()
{
	Input::Update();
	MenuScene::ProcessInput();

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
	ConsoleRenderer::ScreenDrawPlayerWithAnimation(m_fIntroVideo.m_fAxis.X, m_fIntroVideo.m_fAxis.Y, &m_fIntroVideo.m_fanimation[m_fIntroVideo.m_eAnimationState].m_fui[m_fIntroVideo.m_iPlaybackCurrentSeconds], m_fIntroVideo.m_iColor);
}