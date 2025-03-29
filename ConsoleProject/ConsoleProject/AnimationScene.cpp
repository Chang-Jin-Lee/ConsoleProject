#include "AnimationScene.h"
#include "Input.h"
#include "Game.h"
#include "UI.h"

// Time 관련
float m_m_fcurrentTimeAnimationScene = 0;
float m_floadingTimeAnimationScene = 5;
float m_fMenuLastTimeAnimationScene = 0;

float m_finitialOneSecondAnimationScene = 0;
float m_m_fcountOneSecondAnimationScenesAnimationScene = 0;
float m_fPlayer_xAnimationScene = 0;
float m_fPlayer_yAnimationScene = 0;

float m_fFPSTimeAnimationScene = 1 / 60;
//float m_fFPSTimeAnimationScene = 60;
float m_fcountOneSecondAnimationScene = 0;

UI::FUI m_fBackGroundUI;

UI::FUI m_fSpeechBubbleAnimationScene;
UI::FUI m_fCursorAnimationScene;

Object::FPlayerCharacter m_fRapiAnimationScene;
Object::FPlayerCharacter m_fAniscAnimationScene;
Object::FPlayerCharacter m_fNeonAnimationScene;

void AnimationScene::Initialize()	// 게임 시작할 때 초기화
{
	m_fBackGroundUI.m_fAxis.X = ConsoleRenderer::ScreenCenter(m_fBackGroundUI.m_ppcontent[0]);
	m_fBackGroundUI.m_fAxis.Y = ConsoleRenderer::ScreenHeight() / 2 - m_fBackGroundUI.m_ippcontentSize /2 ;
	m_fBackGroundUI.m_iUIColor = FG_WHITE;

	m_fMenuLastTimeAnimationScene = Time::GetTotalTime();
	m_finitialOneSecondAnimationScene = Time::GetTotalTime();
	m_fcountOneSecondAnimationScene = Time::GetTotalTime();

	// Initialize m_fRapiAnimationScene
	Object::SetPlayerAnimationNameFullBody(&m_fRapiAnimationScene, (char*)"RapiFullBodyIdle", (char*)"RapiFullBodyIdleTalk", (char*)"RapiFullBodyExpression");
	Object::LoadAnimationData(&m_fRapiAnimationScene);
	m_fRapiAnimationScene.m_eAnimationState = Object::EAnimationState::FULLBODYIDLE;
	m_fRapiAnimationScene.m_bPlayable = true;
	m_fRapiAnimationScene.m_fAxis.X = ConsoleRenderer::ScreenCenter(m_fRapiAnimationScene.m_fanimation[m_fRapiAnimationScene.m_eAnimationState].m_fui->m_ppcontent[0]);
	m_fRapiAnimationScene.m_fAxis.Y = ConsoleRenderer::ScreenHeight() * 0.32;
	m_fRapiAnimationScene.m_iColor = FG_WHITE;
	

	// Initialize SpeechBubble 
	int SpeechSlateYSize = ConsoleRenderer::ScreenHeight() * 0.3;
	int SpeechSlateXSize = ConsoleRenderer::ScreenWidth();
	m_fSpeechBubbleAnimationScene.m_ppcontent = (char**)malloc(sizeof(char*) * SpeechSlateYSize);
	for (int i = 0; i < SpeechSlateYSize; i++)
	{
		m_fSpeechBubbleAnimationScene.m_ppcontent[i] = (char*)malloc(sizeof(char) * SpeechSlateXSize);
		memset(m_fSpeechBubbleAnimationScene.m_ppcontent[i], ' ', SpeechSlateXSize);
	}
	m_fSpeechBubbleAnimationScene.m_ippcontentSize = SpeechSlateYSize;
	m_fSpeechBubbleAnimationScene.m_fAxis.X = 0;
	m_fSpeechBubbleAnimationScene.m_fAxis.Y = ConsoleRenderer::ScreenHeight() - SpeechSlateYSize;
	m_fSpeechBubbleAnimationScene.m_iUIColor = FG_GRAY;
	
	float BubbleTicknessRatio = 0.005f;
	for (int i = 0; i < SpeechSlateYSize * BubbleTicknessRatio; i++)
	{
		for (int j = 0; j < SpeechSlateXSize; j++)
		{
			m_fSpeechBubbleAnimationScene.m_ppcontent[i][j] = L'⬛';
		}
	}
	for (int i = SpeechSlateYSize * (1-BubbleTicknessRatio); i < SpeechSlateYSize; i++)
	{
		for (int j = 0; j < SpeechSlateXSize; j++)
		{
			m_fSpeechBubbleAnimationScene.m_ppcontent[i][j] = L'⬛';
		}
	}
	for (int j = 0; j < SpeechSlateXSize * BubbleTicknessRatio / 2; j++)
	{
		for (int i = SpeechSlateYSize * BubbleTicknessRatio ; i < SpeechSlateYSize * (1-BubbleTicknessRatio); i++)
		{
			m_fSpeechBubbleAnimationScene.m_ppcontent[i][j] = L'⬛';
		}
	}
	for (int j = SpeechSlateXSize * (1-BubbleTicknessRatio / 2) ; j < SpeechSlateXSize; j++)
	{
		for (int i = SpeechSlateYSize * BubbleTicknessRatio; i < SpeechSlateYSize *( 1-BubbleTicknessRatio); i++)
		{
			m_fSpeechBubbleAnimationScene.m_ppcontent[i][j] = L'⬛';
		}
	}
}

void AnimationScene::LoadData()
{
	//Images/CityForest01/CityForest_01.txt
	//Video/Aru/frame_0001.txt
	if (FileController::FileRead("Images/CityForest01/CityForest_01.txt", "r", &m_fBackGroundUI.m_ppcontent, &m_fBackGroundUI.m_ippcontentSize))
	{
		m_fBackGroundUI.m_fAxis.X = 0;
		m_fBackGroundUI.m_fAxis.Y = 0;
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
	UI::Release(&m_fBackGroundUI);
	UI::Release(&m_fSpeechBubbleAnimationScene);
}

void AnimationScene::Update()
{
	Input::Update();
	AnimationScene::ProcessInput();

	m_fMenuLastTimeAnimationScene = Time::GetTotalTime() - m_m_fcurrentTimeAnimationScene;
	if (m_fMenuLastTimeAnimationScene >= m_floadingTimeAnimationScene + 1)
	{
		//Game::ChangeScene(ESceneState::PLAY);
	}

	m_m_fcountOneSecondAnimationScenesAnimationScene = Time::GetTotalTime() - m_finitialOneSecondAnimationScene;
	if (m_m_fcountOneSecondAnimationScenesAnimationScene >= 1)
	{
		m_finitialOneSecondAnimationScene = Time::GetTotalTime();
		m_fPlayer_xAnimationScene += 3;
		m_m_fcountOneSecondAnimationScenesAnimationScene = 0;
	}

	if (Time::GetTotalTime() - m_fcountOneSecondAnimationScene >= m_fFPSTimeAnimationScene)	// 1/60 초에 한 번씩
	{
		//if (m_fIntroVideo.m_bPlayable)
		//{
		//	m_fIntroVideo.m_iPlaybackCurrentSeconds = (m_fIntroVideo.m_iPlaybackCurrentSeconds + 1) % m_fIntroVideo.m_fanimation[m_fIntroVideo.m_eAnimationState].m_iMaxLength;
		//}
		if (m_fRapiAnimationScene.m_bPlayable)
		{
			m_fRapiAnimationScene.m_iPlaybackCurrentSeconds = (m_fRapiAnimationScene.m_iPlaybackCurrentSeconds + 1) % m_fRapiAnimationScene.m_fanimation[m_fRapiAnimationScene.m_eAnimationState].m_iMaxLength;
		}

		m_fcountOneSecondAnimationScene = Time::GetTotalTime();
	}
}

void AnimationScene::Render()
{
	//ConsoleRenderer::ScreenDrawUIFromFile(&m_fBackGroundUI, FG_WHITE);
	//ConsoleRenderer::ScreenDrawPlayerWithAnimation(m_fIntroVideo.m_fAxis.X, m_fIntroVideo.m_fAxis.Y, &m_fIntroVideo.m_fanimation[m_fIntroVideo.m_eAnimationState].m_fui[m_fIntroVideo.m_iPlaybackCurrentSeconds], &m_fIntroVideo.m_fHealthBar, FG_WHITE);
	ConsoleRenderer::ScreenDrawUIFromFile(&m_fBackGroundUI, m_fBackGroundUI.m_iUIColor);
	ConsoleRenderer::ScreenDrawUIFromFile(&m_fSpeechBubbleAnimationScene, m_fSpeechBubbleAnimationScene.m_iUIColor);

	ConsoleRenderer::ScreenDrawPlayerWithAnimation(m_fRapiAnimationScene.m_fAxis.X, m_fRapiAnimationScene.m_fAxis.Y, &m_fRapiAnimationScene.m_fanimation[m_fRapiAnimationScene.m_eAnimationState].m_fui[m_fRapiAnimationScene.m_iPlaybackCurrentSeconds], m_fRapiAnimationScene.m_iColor);

}