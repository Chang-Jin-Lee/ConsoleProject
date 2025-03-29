#include "AnimationScene.h"
#include "Input.h"
#include "Game.h"
#include "UI.h"

// Time 관련
float m_fcurrentTime = 0;
float m_floadingTime = 5;
float m_fMenuLastTime = 0;

float m_finitialOneSecond = 0;
float m_fcountOneSeconds = 0;
float m_fPlayer_x = 0;
float m_fPlayer_y = 0;

float m_fFPSTimeAnimationScene = 1 / 60;
//float m_fFPSTimeAnimationScene = 60;
float m_fcountOneSecond = 0;
bool bcatBlink = false;

UI::FUI m_fTitleFile;
UI::FUI m_fCursor;
UI::FUI m_fStartButton;
UI::FUI m_fExitButton;

Object::FPlayerCharacter m_fIntroVideo;

void AnimationScene::Initialize()	// 게임 시작할 때 초기화
{
	Object::SetPlayerAnimationName(&m_fIntroVideo, (char*)"RapiLobby");
	Object::LoadAnimationData(&m_fIntroVideo);
	m_fIntroVideo.m_eAnimationState = Object::EAnimationState::FULLBODY_IDLE;
	m_fIntroVideo.m_bPlayable = true;
	//m_fIntroVideo.m_fAxis.X = ConsoleRenderer::ScreenCenter(m_fIntroVideo.m_fanimation[m_fIntroVideo.m_eAnimationState].m_fui->m_ppcontent[0]);
	//m_fIntroVideo.m_fAxis.Y = ConsoleRenderer::ScreenHeight() * 0.1;

	m_fIntroVideo.m_fAxis.X = 0;
	m_fIntroVideo.m_fAxis.Y = 0;
	m_fIntroVideo.m_iPlaybackCurrentSeconds = 0;

	m_fMenuLastTime = Time::GetTotalTime();
	m_finitialOneSecond = Time::GetTotalTime();
	m_fcountOneSecond = Time::GetTotalTime();
}

void AnimationScene::LoadData()
{
	//Images/CityForest01/CityForest_01.txt
	//Video/Aru/frame_0001.txt
	if (FileController::FileRead("Images/CityForest01/CityForest_01.txt", "r", &m_fTitleFile.m_ppcontent, &m_fTitleFile.m_ippcontentSize))
	{
		m_fTitleFile.m_fAxis.X = 0;
		m_fTitleFile.m_fAxis.Y = 0;
	}
}

void AnimationScene::ProcessInput()
{
	if (Input::IsKeyPressed(VK_ESCAPE))  //종료
	{
		Game::GameExit();
	}

	if (Input::IsKeyPressed(VK_RETURN) || Input::IsKeyPressed(VK_SPACE))
	{
		Game::ChangeScene(ESceneState::PLAY);
	}

	if (Input::IsKeyPressed(VK_NUMPAD0))
	{
		int fontsize = ConsoleRenderer::GetScreenFontSize();
		ConsoleRenderer::SetScreenFontSize(fontsize + 1);
		ConsoleRenderer::ScreenInit();
	}

	if (Input::IsKeyPressed(VK_NUMPAD9))
	{
		int fontsize = ConsoleRenderer::GetScreenFontSize();
		ConsoleRenderer::SetScreenFontSize(fontsize - 1);
		ConsoleRenderer::ScreenInit();
	}
}

void AnimationScene::Release()
{
	UI::Release(&m_fTitleFile);
	Object::Release(&m_fIntroVideo);
}

void AnimationScene::Update()
{
	Input::Update();
	AnimationScene::ProcessInput();

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

	if (Time::GetTotalTime() - m_fcountOneSecond >= m_fFPSTimeAnimationScene)	// 0.5초에 한 번씩
	{
		if (m_fIntroVideo.m_bPlayable)
		{
			m_fIntroVideo.m_iPlaybackCurrentSeconds = (m_fIntroVideo.m_iPlaybackCurrentSeconds + 1) % m_fIntroVideo.m_fanimation[m_fIntroVideo.m_eAnimationState].m_iMaxLength;
		}
		m_fcountOneSecond = Time::GetTotalTime();
	}
}

void AnimationScene::Render()
{
	//ConsoleRenderer::ScreenDrawUIFromFile(&m_fTitleFile, FG_WHITE);
	ConsoleRenderer::ScreenDrawPlayerWithAnimation(m_fIntroVideo.m_fAxis.X, m_fIntroVideo.m_fAxis.Y, &m_fIntroVideo.m_fanimation[m_fIntroVideo.m_eAnimationState].m_fui[m_fIntroVideo.m_iPlaybackCurrentSeconds], &m_fIntroVideo.m_fHealthBar, FG_WHITE);
}