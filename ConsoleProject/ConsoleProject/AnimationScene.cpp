#include "AnimationScene.h"
#include "Input.h"
#include "Game.h"
#include "UI.h"
#include <fmod.hpp>

#pragma comment(lib, "fmod_vc.lib")

// Time 관련
float m_m_fcurrentTimeAnimationScene = 0;
float m_floadingTimeAnimationScene = 5;
float m_fMenuLastTimeAnimationScene = 0;

float m_finitialOneSecondAnimationScene = 0;
float m_m_fcountOneSecondAnimationScenesAnimationScene = 0;
float m_fPlayer_xAnimationScene = 0;
float m_fPlayer_yAnimationScene = 0;

float m_fFPSTimeAnimationScene = 1 / 60;
float m_fFPSLastTimeAnimationScene = 0;
float m_fcountOneSecondAnimationScene = 0;

// 말풍선, 선택창 UI
UI::FUI m_fSpeechBubbleAnimationScene;

Object::FPlayerCharacter m_fShiftyAnimationScene;
Object::FPlayerCharacter m_fRapiAnimationScene;
Object::FPlayerCharacter m_fAniscAnimationScene;
Object::FPlayerCharacter m_fNeonAnimationScene;

ECharacterName ch1 = Rapi, ch2 = None;

UI::FUI m_fBackGroundUI;
UI::FGAMEDIALOGANIMATIONSCENE m_fgameDialog[MAX_DIALOG_SIZE];
int m_igameDialogIndex = 1;
int m_fgameDialogSize = 15;

// Sound
FMOD::System* systemAnimationScene = nullptr;
FMOD::Sound* soundAnimationScene1 = nullptr;
FMOD::Sound* soundAnimationScene2 = nullptr;
FMOD::Channel* channelAnimationScene = nullptr;
bool bSecondMusic = true;

void AnimationScene::Initialize()	// 게임 시작할 때 초기화
{
	m_fBackGroundUI.m_iUIColor = FG_WHITE;

	m_igameDialogIndex = 1;

	m_fMenuLastTimeAnimationScene = Time::GetTotalTime();
	m_finitialOneSecondAnimationScene = Time::GetTotalTime();
	m_fcountOneSecondAnimationScene = Time::GetTotalTime();
	m_fFPSLastTimeAnimationScene = Time::GetTotalTime();

	// Initialize m_fShiftyAnimationScene
	m_fShiftyAnimationScene.m_eAnimationState = Object::EAnimationState::FULLBODYIDLE;
	m_fShiftyAnimationScene.m_bPlayable = true;
	m_fShiftyAnimationScene.m_fAxis.X = (SHORT)(ConsoleRenderer::ScreenWidth() * 0.02);
	m_fShiftyAnimationScene.m_fAxis.Y = (SHORT)(ConsoleRenderer::ScreenHeight() * 0.01);
	m_fShiftyAnimationScene.m_iColor = FG_WHITE;
	m_fShiftyAnimationScene.m_bVisible = false;

	// Initialize m_fRapiAnimationScene
	m_fRapiAnimationScene.m_eAnimationState = Object::EAnimationState::FULLBODYEXPRESSION;
	m_fRapiAnimationScene.m_bPlayable = true;
	m_fRapiAnimationScene.m_fAxis.X = ConsoleRenderer::ScreenCenter(m_fRapiAnimationScene.m_fanimation[m_fRapiAnimationScene.m_eAnimationState].m_fui->m_ppcontent[0]);
	m_fRapiAnimationScene.m_fAxis.Y = (SHORT)(ConsoleRenderer::ScreenHeight() * 0.12);
	m_fRapiAnimationScene.m_iColor = FG_WHITE;

	// Initialize m_fAniscAnimationScene
	m_fAniscAnimationScene.m_eAnimationState = Object::EAnimationState::FULLBODYEXPRESSION;
	m_fAniscAnimationScene.m_bPlayable = true;
	m_fAniscAnimationScene.m_fAxis.X = ConsoleRenderer::ScreenCenter(m_fAniscAnimationScene.m_fanimation[m_fAniscAnimationScene.m_eAnimationState].m_fui->m_ppcontent[0]);
	//m_fAniscAnimationScene.m_fAxis.X = 0;
	m_fAniscAnimationScene.m_fAxis.Y = (SHORT)(ConsoleRenderer::ScreenHeight() * 0.12);
	m_fAniscAnimationScene.m_iColor = FG_WHITE;

	// Initialize m_fNeonAnimationScene
	m_fNeonAnimationScene.m_eAnimationState = Object::EAnimationState::FULLBODYEXPRESSION;
	m_fNeonAnimationScene.m_bPlayable = true;
	m_fNeonAnimationScene.m_fAxis.X = ConsoleRenderer::ScreenCenter(m_fNeonAnimationScene.m_fanimation[m_fNeonAnimationScene.m_eAnimationState].m_fui->m_ppcontent[0]);
	m_fNeonAnimationScene.m_fAxis.Y = (SHORT)(ConsoleRenderer::ScreenHeight() * 0.12);
	m_fNeonAnimationScene.m_iColor = FG_WHITE;

	// Initialize SpeechBubble 
	UI::CreateBubbleUI(&m_fSpeechBubbleAnimationScene,
		(int)(ConsoleRenderer::ScreenWidth()),
		(int)(ConsoleRenderer::ScreenHeight() * 0.3),
		0,
		(int)(ConsoleRenderer::ScreenHeight() * 0.7),
		0.01f,
		FG_WHITE
	);

	if (FileController::FileReadFromCSV_Dialogue("Dialogue", "r", &m_fSpeechBubbleAnimationScene, m_fgameDialog, &m_fgameDialogSize))
	{

	}
	systemAnimationScene->playSound(soundAnimationScene1, nullptr, false, &channelAnimationScene);
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

	Object::SetPlayerAnimationNameFullBody(&m_fShiftyAnimationScene, (char*)"ShiftyFullBodyIdle");
	Object::LoadAnimationDataWithFrame(&m_fShiftyAnimationScene, 0.01f);

	Object::SetPlayerAnimationNameFullBody(&m_fRapiAnimationScene, (char*)"RapiFullBodyIdle", (char*)"RapiFullBodyIdleTalk", (char*)"RapiFullBodyExpression");
	Object::LoadAnimationData(&m_fRapiAnimationScene);

	Object::SetPlayerAnimationNameFullBody(&m_fAniscAnimationScene, (char*)"AnisFullBodyIdle", (char*)"AnisFullBodyIdleTalk", (char*)"AnisFullBodyExpression");
	Object::LoadAnimationData(&m_fAniscAnimationScene);

	Object::SetPlayerAnimationNameFullBody(&m_fNeonAnimationScene, (char*)"NeonFullBodyIdle", (char*)"NeonFullBodyIdleTalk", (char*)"NeonFullBodyExpression");
	Object::LoadAnimationData(&m_fNeonAnimationScene);

	// FMOD 시스템 초기화
	if (FMOD::System_Create(&systemAnimationScene) != FMOD_OK)
	{
		ConsoleRenderer::print((char*)"System_Create fail");
	}
	systemAnimationScene->init(512, FMOD_INIT_NORMAL, nullptr);

	// 사운드 1 미리 로드
	if (systemAnimationScene->createSound("Music/bgm_AnimationScene.mp3", FMOD_DEFAULT, nullptr, &soundAnimationScene1) != FMOD_OK)
		ConsoleRenderer::print((char*)"createSound 1 fail");

	// 사운드 2 미리 로드
	if (systemAnimationScene->createSound("Music/bgm_AnimationScene2.mp3", FMOD_DEFAULT, nullptr, &soundAnimationScene2) != FMOD_OK)
		ConsoleRenderer::print((char*)"createSound 2 fail");
}

void AnimationScene::ProcessInput()
{
	if (Input::IsKeyPressed(VK_ESCAPE))  //종료
	{
		Game::GameExit();
	}

	if (Input::IsKeyPressed(VK_Z))
	{
		Game::ChangeScene(ESceneState::PLAY);
	}

	if (Input::IsKeyPressed(VK_SPACE) || Input::IsKeyPressed(VK_RETURN))
	{
		if (m_fgameDialog[m_igameDialogIndex].m_iTalkingCharacterSize >= 2)
		{
			m_fShiftyAnimationScene.m_bVisible = true;
		}
		else
		{
			m_fShiftyAnimationScene.m_bVisible = false;
		}

		if (m_fgameDialog[m_igameDialogIndex].NextIdx == 0)
		{
			m_igameDialogIndex = m_fgameDialog[m_igameDialogIndex].SelectNextDialogue[m_fgameDialog[m_igameDialogIndex].m_iselectIndex];
		}
		else if(m_fgameDialog[m_igameDialogIndex].NextIdx == -1)
		{
			Game::ChangeScene(ESceneState::PLAY);
		}
		else
		{
			m_igameDialogIndex = m_fgameDialog[m_igameDialogIndex].NextIdx % m_fgameDialogSize;
			char numberName[10];
			sprintf_s(numberName, sizeof(numberName), "%04d", m_fgameDialog[m_igameDialogIndex].NextIdx);
			ConsoleRenderer::print(numberName);
		}
	}

	if (Input::IsKeyPressed(VK_BACK))
	{
		m_igameDialogIndex - 1 < 0 ? m_igameDialogIndex = m_fgameDialogSize - 1 : m_igameDialogIndex--;
	}

	if (Input::IsKeyPressed(VK_UP))
	{
		m_fgameDialog[m_igameDialogIndex].m_iselectIndex - 1 < 0 ? m_fgameDialog[m_igameDialogIndex].m_iselectIndex = MAX_SELECTBUBBLE_SIZE - 1 : m_fgameDialog[m_igameDialogIndex].m_iselectIndex--;
	}

	if (Input::IsKeyPressed(VK_DOWN))
	{
		m_fgameDialog[m_igameDialogIndex].m_iselectIndex = (m_fgameDialog[m_igameDialogIndex].m_iselectIndex + 1) % MAX_SELECTBUBBLE_SIZE;
	}

	//if (Input::IsKeyPressed(VK_RETURN) || Input::IsKeyPressed(VK_SPACE))
	//{
	//	Game::ChangeScene(ESceneState::PLAY);
	//}
	if (Input::IsKeyPressed(VK_T))  // 캐릭터 변경
	{
		switch (ch1)
		{
		case Rapi:
			ch1 = Anis;
			break;
		case Anis:
			ch1 = Neon;
			break;
		case Neon:
			ch1 = None;
			break;
		case None:
			ch1 = Rapi;
			break;
		default:
			break;
		}
	}

	if (Input::IsKeyPressed(VK_R))  //종료
	{
		switch (m_fRapiAnimationScene.m_eAnimationState)
		{
		case Object::EAnimationState::FULLBODYIDLE:
			m_fRapiAnimationScene.m_iPlaybackCurrentSeconds = 0;
			m_fAniscAnimationScene.m_iPlaybackCurrentSeconds = 0;
			m_fNeonAnimationScene.m_iPlaybackCurrentSeconds = 0;
			m_fRapiAnimationScene.m_eAnimationState = Object::EAnimationState::FULLBODYTALK;
			m_fAniscAnimationScene.m_eAnimationState = Object::EAnimationState::FULLBODYTALK;
			m_fNeonAnimationScene.m_eAnimationState = Object::EAnimationState::FULLBODYTALK;
			break;
		case Object::EAnimationState::FULLBODYTALK:
			m_fRapiAnimationScene.m_iPlaybackCurrentSeconds = 0;
			m_fAniscAnimationScene.m_iPlaybackCurrentSeconds = 0;
			m_fNeonAnimationScene.m_iPlaybackCurrentSeconds = 0;
			m_fRapiAnimationScene.m_eAnimationState = Object::EAnimationState::FULLBODYEXPRESSION;
			m_fAniscAnimationScene.m_eAnimationState = Object::EAnimationState::FULLBODYEXPRESSION;
			m_fNeonAnimationScene.m_eAnimationState = Object::EAnimationState::FULLBODYEXPRESSION;
			break;
		case Object::EAnimationState::FULLBODYEXPRESSION:
			m_fRapiAnimationScene.m_iPlaybackCurrentSeconds = 0;
			m_fAniscAnimationScene.m_iPlaybackCurrentSeconds = 0;
			m_fNeonAnimationScene.m_iPlaybackCurrentSeconds = 0;
			m_fRapiAnimationScene.m_eAnimationState = Object::EAnimationState::FULLBODYIDLE;
			m_fAniscAnimationScene.m_eAnimationState = Object::EAnimationState::FULLBODYIDLE;
			m_fNeonAnimationScene.m_eAnimationState = Object::EAnimationState::FULLBODYIDLE;
			break;
		default:
			break;
		}
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

void AnimationScene::Release()
{
	UI::Release(&m_fBackGroundUI);
	UI::Release(&m_fSpeechBubbleAnimationScene);
	Object::Release(&m_fRapiAnimationScene);
	Object::Release(&m_fAniscAnimationScene);
	Object::Release(&m_fNeonAnimationScene);
	soundAnimationScene1->release();
	soundAnimationScene2->release();
	systemAnimationScene->close();
	systemAnimationScene->release();
}

void AnimationScene::Update()
{
	Input::Update();
	AnimationScene::ProcessInput();

	if (m_igameDialogIndex == 7 && bSecondMusic)
	{
		if (channelAnimationScene)
			channelAnimationScene->stop();

		// 두 번째 사운드 재생
		bSecondMusic = false;
		systemAnimationScene->playSound(soundAnimationScene2, nullptr, false, &channelAnimationScene);
	}

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

	for (int i = 0; i < MAX_SELECTBUBBLE_SIZE; i++)
	{
		if(i == m_fgameDialog[m_igameDialogIndex].m_iselectIndex)
			m_fgameDialog[m_igameDialogIndex].m_fSelectBubble[i].m_fbackGround.m_iUIColor = FG_WHITE;
		else
			m_fgameDialog[m_igameDialogIndex].m_fSelectBubble[i].m_fbackGround.m_iUIColor = FG_RED;
	}

	m_fFPSLastTimeAnimationScene = Time::GetTotalTime() - m_fcountOneSecondAnimationScene;
	if (m_fFPSLastTimeAnimationScene >= m_fFPSTimeAnimationScene)	// 1/60 초에 한 번씩
	{
		m_fcountOneSecondAnimationScene = Time::GetTotalTime();

		if (m_fShiftyAnimationScene.m_bPlayable)
			m_fShiftyAnimationScene.m_iPlaybackCurrentSeconds = (m_fShiftyAnimationScene.m_iPlaybackCurrentSeconds + 1) % m_fShiftyAnimationScene.m_fanimation[m_fShiftyAnimationScene.m_eAnimationState].m_iMaxLength;
		if (m_fRapiAnimationScene.m_bPlayable)
			m_fRapiAnimationScene.m_iPlaybackCurrentSeconds = (m_fRapiAnimationScene.m_iPlaybackCurrentSeconds + 1) % m_fRapiAnimationScene.m_fanimation[m_fRapiAnimationScene.m_eAnimationState].m_iMaxLength;
		if (m_fAniscAnimationScene.m_bPlayable)
			m_fAniscAnimationScene.m_iPlaybackCurrentSeconds = (m_fAniscAnimationScene.m_iPlaybackCurrentSeconds + 1) % m_fAniscAnimationScene.m_fanimation[m_fAniscAnimationScene.m_eAnimationState].m_iMaxLength;
		if (m_fNeonAnimationScene.m_bPlayable)
			m_fNeonAnimationScene.m_iPlaybackCurrentSeconds = (m_fNeonAnimationScene.m_iPlaybackCurrentSeconds + 1) % m_fNeonAnimationScene.m_fanimation[m_fNeonAnimationScene.m_eAnimationState].m_iMaxLength;
	}
}

void AnimationScene::Render()
{
	if (m_fgameDialog[m_igameDialogIndex].m_aSpeakerTalkable[ECharacterName::Rapi] == true)
		ConsoleRenderer::ScreenDrawPlayerWithAnimation(m_fRapiAnimationScene.m_fAxis.X, m_fRapiAnimationScene.m_fAxis.Y, &m_fRapiAnimationScene.m_fanimation[m_fRapiAnimationScene.m_eAnimationState].m_fui[m_fRapiAnimationScene.m_iPlaybackCurrentSeconds], m_fRapiAnimationScene.m_iColor);
	if (m_fgameDialog[m_igameDialogIndex].m_aSpeakerTalkable[ECharacterName::Anis] == true)
		ConsoleRenderer::ScreenDrawPlayerWithAnimation(m_fAniscAnimationScene.m_fAxis.X, m_fAniscAnimationScene.m_fAxis.Y, &m_fAniscAnimationScene.m_fanimation[m_fAniscAnimationScene.m_eAnimationState].m_fui[m_fAniscAnimationScene.m_iPlaybackCurrentSeconds], m_fAniscAnimationScene.m_iColor);
	if (m_fgameDialog[m_igameDialogIndex].m_aSpeakerTalkable[ECharacterName::Neon] == true)
		ConsoleRenderer::ScreenDrawPlayerWithAnimation(m_fNeonAnimationScene.m_fAxis.X, m_fNeonAnimationScene.m_fAxis.Y, &m_fNeonAnimationScene.m_fanimation[m_fNeonAnimationScene.m_eAnimationState].m_fui[m_fNeonAnimationScene.m_iPlaybackCurrentSeconds], m_fNeonAnimationScene.m_iColor);
	if (m_fgameDialog[m_igameDialogIndex].m_aSpeakerTalkable[ECharacterName::Shifty] == true)
		ConsoleRenderer::ScreenDrawPlayerWithAnimation(m_fShiftyAnimationScene.m_fAxis.X, m_fShiftyAnimationScene.m_fAxis.Y, &m_fShiftyAnimationScene.m_fanimation[m_fShiftyAnimationScene.m_eAnimationState].m_fui[m_fShiftyAnimationScene.m_iPlaybackCurrentSeconds], m_fShiftyAnimationScene.m_iColor);
	ConsoleRenderer::ScreenDrawUIFromFile(&m_fSpeechBubbleAnimationScene, m_fSpeechBubbleAnimationScene.m_iUIColor);

	if (m_fgameDialog[m_igameDialogIndex].NextIdx == 0 || m_fgameDialog[m_igameDialogIndex].NextIdx == -1)
	{
		for (int i = 0; i < MAX_SELECTBUBBLE_SIZE; i++)
		{
			ConsoleRenderer::ScreenDrawUIFromFile(&m_fgameDialog[m_igameDialogIndex].m_fSelectBubble[i].m_fbackGround, m_fgameDialog[m_igameDialogIndex].m_fSelectBubble[i].m_fbackGround.m_iUIColor);
			ConsoleRenderer::ScreenDrawUIFromFile(&m_fgameDialog[m_igameDialogIndex].m_fSelectBubble[i].m_fcontent, m_fgameDialog[m_igameDialogIndex].m_fSelectBubble[i].m_fcontent.m_iUIColor);
		}
	}

	ConsoleRenderer::ScreenDrawUIFromFile(&m_fgameDialog[m_igameDialogIndex].m_fspeaker, m_fgameDialog[m_igameDialogIndex].m_fspeaker.m_iUIColor);
	ConsoleRenderer::ScreenDrawUIFromFile(&m_fgameDialog[m_igameDialogIndex].m_fDialogue, m_fgameDialog[m_igameDialogIndex].m_fDialogue.m_iUIColor);
}