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
float m_fFPSLastTimeAnimationScene = 0;
float m_fcountOneSecondAnimationScene = 0;

// 말풍선, 선택창 UI
UI::FUI m_fSpeechBubbleAnimationScene;
UI::FBUBBLEUI m_fSelectBubbleAnimationScene[MAX_SELECTBUBBLE_SIZE];
UI::FUI m_fCursorAnimationScene;
int m_iselectIndex = 0;

Object::FPlayerCharacter m_fShiftyAnimationScene;
Object::FPlayerCharacter m_fRapiAnimationScene;
Object::FPlayerCharacter m_fAniscAnimationScene;
Object::FPlayerCharacter m_fNeonAnimationScene;

ECharacterName ch1 = Rapi, ch2 = None;

UI::FUI m_fBackGroundUI;
UI::FGAMEDIALOGANIMATIONSCENE m_fgameDialog[MAX_DIALOG_SIZE];
int m_igameDialogIndex = 1;
int m_fgameDialogSize = 15;

void AnimationScene::Initialize()	// 게임 시작할 때 초기화
{
	m_fBackGroundUI.m_iUIColor = FG_WHITE;

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
	m_fShiftyAnimationScene.m_bVisible = true;

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
		//m_fgameDialog[m_igameDialogIndex].m_fDialogue.m_fAxis.X = 0;
		//m_fgameDialog[m_igameDialogIndex].m_fDialogue.m_fAxis.Y = 0;
	}

	// Initialize SelectBubble
	for (int i = 0; i < MAX_SELECTBUBBLE_SIZE; i++)
	{
		UI::CreateBubbleUI(&m_fSelectBubbleAnimationScene[i].m_fbackGround,
			(int)(ConsoleRenderer::ScreenWidth() * 0.3),
			(int)(ConsoleRenderer::ScreenHeight() * 0.05),
			(int)(ConsoleRenderer::ScreenWidth() * 0.65),
			(int)(ConsoleRenderer::ScreenHeight() * (0.55 + 0.06*float(i))),
			0.01f,
			FG_RED
		);
		UI::CreateBubbleUIContent(&m_fSelectBubbleAnimationScene[i], "Images/text/text_anis_30.txt", FG_WHITE);
	}
	
	// Initialize Speech 
	/*m_fSpeechContentIndex = 1;
	m_fSpeechCursorPlayScene = UI::FUI(int(ConsoleRenderer::ScreenWidth() * 0.4), int(ConsoleRenderer::ScreenHeight() * 0.7), (char*)" > ");

	int SpeechSlateYSize = ConsoleRenderer::ScreenHeight() * 0.3;
	int SpeechSlateXSize = ConsoleRenderer::ScreenWidth();
	m_fSpeechSlate.m_ppcontent = (char**)malloc(sizeof(char*) * SpeechSlateYSize);
	for (int i = 0; i < SpeechSlateYSize; i++)
		m_fSpeechSlate.m_ppcontent[i] = (char*)malloc(sizeof(char) * SpeechSlateXSize);
	m_fSpeechSlate.m_ippcontentSize = SpeechSlateYSize;
	m_fSpeechSlate.m_fAxis.X = 0;
	m_fSpeechSlate.m_fAxis.Y = ConsoleRenderer::ScreenHeight() - SpeechSlateYSize;
	for (int i = 0; i < SpeechSlateYSize; i++)
	{
		for (int j = 0; j < SpeechSlateXSize; j++)
		{
			if (i == 0 || i == SpeechSlateYSize - 1)
				m_fSpeechSlate.m_ppcontent[i][j] = '-';
			else if (j == 0 || j == SpeechSlateXSize - 1)
				m_fSpeechSlate.m_ppcontent[i][j] = '|';
			else
				m_fSpeechSlate.m_ppcontent[i][j] = ' ';
		}
	}

	m_fSpeechNextCursor = UI::FUI(int(ConsoleRenderer::ScreenWidth() * 0.95), int(ConsoleRenderer::ScreenHeight() * 0.9), (char*)m_speechNextString);
	for (int i = 0; i < MAX_DIALOG_SIZE; i++)
	{
		m_fgameDialog[i].Dialog.m_fAxis.X = m_fSpeechSlate.m_fAxis.X + ConsoleRenderer::ScreenWidth() * 0.2;
		m_fgameDialog[i].Dialog.m_fAxis.Y = m_fSpeechSlate.m_fAxis.Y + SpeechSlateYSize * 0.5;

		m_fgameDialog[i].Speaker.m_fAxis.X = m_fSpeechSlate.m_fAxis.X + ConsoleRenderer::ScreenWidth() * 0.1;
		m_fgameDialog[i].Speaker.m_fAxis.Y = m_fSpeechSlate.m_fAxis.Y + SpeechSlateYSize * 0.3;

		m_fgameDialog[i].Type.m_fAxis.X = m_fSpeechSlate.m_fAxis.X + ConsoleRenderer::ScreenWidth() * 0.8;
		m_fgameDialog[i].Type.m_fAxis.Y = m_fSpeechSlate.m_fAxis.Y + SpeechSlateYSize * 0.3;

		m_fgameDialog[i].Likeability.m_fAxis.X = ConsoleRenderer::ScreenWidth() * 0.9;
		m_fgameDialog[i].Likeability.m_fAxis.Y = ConsoleRenderer::ScreenHeight() * 0.1;
	}*/
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

	//if (FileController::FileReadFromCSV("Dialogue.csv", "r", m_fgameDialog, &m_fgameDialogSize) == false)
	//	ConsoleRenderer::print((char*)"PlayScene_FileReadError\n");

	/*for (int i = 0; i < MAX_SELECTBUBBLE_SIZE; i++)
	{
		UI::CreateBubbleUI(&m_fSelectBubbleAnimationScene[i].m_fbackGround,
			(int)(ConsoleRenderer::ScreenWidth() * 0.3),
			(int)(ConsoleRenderer::ScreenHeight() * 0.05),
			(int)(ConsoleRenderer::ScreenWidth() * 0.65),
			(int)(ConsoleRenderer::ScreenHeight() * (0.55 + 0.06 * float(i))),
			0.01f,
			FG_GRAY
		);
	}*/
}

void AnimationScene::ProcessInput()
{
	if (Input::IsKeyPressed(VK_ESCAPE))  //종료
	{
		Game::GameExit();
	}

	if (Input::IsKeyPressed(VK_SPACE) || Input::IsKeyPressed(VK_RETURN))
	{
		m_igameDialogIndex = (m_igameDialogIndex + 1) % m_fgameDialogSize;
	}

	if (Input::IsKeyPressed(VK_BACK))
	{
		m_igameDialogIndex - 1 < 0 ? m_igameDialogIndex = m_fgameDialogSize - 1 : m_igameDialogIndex--;
	}

	if (Input::IsKeyPressed(VK_UP))
	{
		m_iselectIndex - 1 < 0 ? m_iselectIndex = MAX_SELECTBUBBLE_SIZE - 1 : m_iselectIndex--;
	}

	if (Input::IsKeyPressed(VK_DOWN))
	{
		m_iselectIndex = (m_iselectIndex + 1) % MAX_SELECTBUBBLE_SIZE;
	}

	if (Input::IsKeyPressed(VK_RETURN) || Input::IsKeyPressed(VK_SPACE))
	{
		Game::ChangeScene(ESceneState::PLAY);
	}
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


	//UI::Release(&m_fSpeechSlate);
	//for (int i = 0; i < MAX_DIALOG_SIZE; i++)
	//{
	//	UI::Release(&m_fgameDialog[i].Speaker);
	//	UI::Release(&m_fgameDialog[i].Answer);
	//	UI::Release(&m_fgameDialog[i].Dialog);
	//	UI::Release(&m_fgameDialog[i].Likeability);
	//	UI::Release(&m_fgameDialog[i].SceneName);
	//	UI::Release(&m_fgameDialog[i].Type);
	//}
	//UI::Release(&m_fplaySceneobj.CatIdle);
	//UI::Release(&m_fplaySceneobj.CatCurios);
	//UI::Release(&m_fplaySceneobj.CatEyeOpen);
	//UI::Release(&m_fplaySceneobj.CatEyeOpen_TailDown);
	//UI::Release(&m_fplaySceneobj.CatFace);
	//UI::Release(&m_fplaySceneobj.CatJump);
	//UI::Release(&m_fplaySceneobj.CatOnRoof);
	//UI::Release(&m_fplaySceneobj.CatScared);
	//UI::Release(&m_fplaySceneobj.CatWalking);
	//UI::Release(&m_fplaySceneobj.CatWeird);
	//UI::Release(&m_fplaySceneobj.BirdAttack);
	//UI::Release(&m_fplaySceneobj.BirdIdle);
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

	for (int i = 0; i < MAX_SELECTBUBBLE_SIZE; i++)
	{
		if(i == m_iselectIndex)
			m_fSelectBubbleAnimationScene[i].m_fbackGround.m_iUIColor = FG_WHITE;
		else
			m_fSelectBubbleAnimationScene[i].m_fbackGround.m_iUIColor = FG_RED;
	}

	m_fFPSLastTimeAnimationScene = Time::GetTotalTime() - m_fcountOneSecondAnimationScene;
	if (m_fFPSLastTimeAnimationScene >= m_fFPSTimeAnimationScene)	// 1/60 초에 한 번씩
	{
		m_fcountOneSecondAnimationScene = Time::GetTotalTime();
		//if (m_fIntroVideo.m_bPlayable)
		//{
		//	m_fIntroVideo.m_iPlaybackCurrentSeconds = (m_fIntroVideo.m_iPlaybackCurrentSeconds + 1) % m_fIntroVideo.m_fanimation[m_fIntroVideo.m_eAnimationState].m_iMaxLength;
		//}

		if (m_fShiftyAnimationScene.m_bPlayable)
		{
			m_fShiftyAnimationScene.m_iPlaybackCurrentSeconds = (m_fShiftyAnimationScene.m_iPlaybackCurrentSeconds + 1) % m_fShiftyAnimationScene.m_fanimation[m_fShiftyAnimationScene.m_eAnimationState].m_iMaxLength;
		}
		if (m_fRapiAnimationScene.m_bPlayable)
		{
			m_fRapiAnimationScene.m_iPlaybackCurrentSeconds = (m_fRapiAnimationScene.m_iPlaybackCurrentSeconds + 1) % m_fRapiAnimationScene.m_fanimation[m_fRapiAnimationScene.m_eAnimationState].m_iMaxLength;
		}
		if (m_fAniscAnimationScene.m_bPlayable)
		{
			m_fAniscAnimationScene.m_iPlaybackCurrentSeconds = (m_fAniscAnimationScene.m_iPlaybackCurrentSeconds + 1) % m_fAniscAnimationScene.m_fanimation[m_fAniscAnimationScene.m_eAnimationState].m_iMaxLength;
		}
		if (m_fNeonAnimationScene.m_bPlayable)
		{
			m_fNeonAnimationScene.m_iPlaybackCurrentSeconds = (m_fNeonAnimationScene.m_iPlaybackCurrentSeconds + 1) % m_fNeonAnimationScene.m_fanimation[m_fNeonAnimationScene.m_eAnimationState].m_iMaxLength;
		}
	}
}

void AnimationScene::Render()
{
	//ConsoleRenderer::ScreenDrawUIFromFile(&m_fBackGroundUI, FG_WHITE);
	//ConsoleRenderer::ScreenDrawPlayerWithAnimation(m_fIntroVideo.m_fAxis.X, m_fIntroVideo.m_fAxis.Y, &m_fIntroVideo.m_fanimation[m_fIntroVideo.m_eAnimationState].m_fui[m_fIntroVideo.m_iPlaybackCurrentSeconds], &m_fIntroVideo.m_fHealthBar, FG_WHITE);
	//ConsoleRenderer::ScreenDrawUIFromFile(&m_fBackGroundUI, m_fBackGroundUI.m_iUIColor);
	
	//ConsoleRenderer::ScreenDrawUIFromFile(&m_fSpeechSlate, FG_SKY);
	//ConsoleRenderer::ScreenDrawUI(&m_fSpeechNextCursor, FG_WHITE);

	//ConsoleRenderer::ScreenDrawUI(&m_fgameDialog[m_fSpeechContentIndex].Speaker, FG_WHITE);
	//ConsoleRenderer::ScreenDrawUI(&m_fgameDialog[m_fSpeechContentIndex].Dialog, FG_WHITE);
	//ConsoleRenderer::ScreenDrawUI(&m_fgameDialog[m_fSpeechContentIndex].Type, FG_WHITE);
	//ConsoleRenderer::ScreenDrawUI(&m_fgameDialog[m_fSpeechContentIndex].Likeability, FG_WHITE);

	if(ch1 == Rapi || ch2 == Rapi)
		ConsoleRenderer::ScreenDrawPlayerWithAnimation(m_fRapiAnimationScene.m_fAxis.X, m_fRapiAnimationScene.m_fAxis.Y, &m_fRapiAnimationScene.m_fanimation[m_fRapiAnimationScene.m_eAnimationState].m_fui[m_fRapiAnimationScene.m_iPlaybackCurrentSeconds], m_fRapiAnimationScene.m_iColor);
	if (ch1 == Anis || ch2 == Anis)
	ConsoleRenderer::ScreenDrawPlayerWithAnimation(m_fAniscAnimationScene.m_fAxis.X, m_fAniscAnimationScene.m_fAxis.Y, &m_fAniscAnimationScene.m_fanimation[m_fAniscAnimationScene.m_eAnimationState].m_fui[m_fAniscAnimationScene.m_iPlaybackCurrentSeconds], m_fAniscAnimationScene.m_iColor);
	if (ch1 == Neon || ch2 == Neon)
		ConsoleRenderer::ScreenDrawPlayerWithAnimation(m_fNeonAnimationScene.m_fAxis.X, m_fNeonAnimationScene.m_fAxis.Y, &m_fNeonAnimationScene.m_fanimation[m_fNeonAnimationScene.m_eAnimationState].m_fui[m_fNeonAnimationScene.m_iPlaybackCurrentSeconds], m_fNeonAnimationScene.m_iColor);
	
	if(m_fShiftyAnimationScene.m_bVisible)
		ConsoleRenderer::ScreenDrawPlayerWithAnimation(m_fShiftyAnimationScene.m_fAxis.X, m_fShiftyAnimationScene.m_fAxis.Y, &m_fShiftyAnimationScene.m_fanimation[m_fShiftyAnimationScene.m_eAnimationState].m_fui[m_fShiftyAnimationScene.m_iPlaybackCurrentSeconds], m_fShiftyAnimationScene.m_iColor);
	
	ConsoleRenderer::ScreenDrawUIFromFile(&m_fSpeechBubbleAnimationScene, m_fSpeechBubbleAnimationScene.m_iUIColor);

	for (int i = 0; i < MAX_SELECTBUBBLE_SIZE; i++)
	{
		ConsoleRenderer::ScreenDrawUIFromFile(&m_fSelectBubbleAnimationScene[i].m_fbackGround, m_fSelectBubbleAnimationScene[i].m_fbackGround.m_iUIColor);
		ConsoleRenderer::ScreenDrawUIFromFile(&m_fSelectBubbleAnimationScene[i].m_fcontent, m_fSelectBubbleAnimationScene[i].m_fcontent.m_iUIColor);
	}

	ConsoleRenderer::ScreenDrawUIFromFile(&m_fgameDialog[m_igameDialogIndex].m_fspeaker, m_fgameDialog[m_igameDialogIndex].m_fspeaker.m_iUIColor);
	ConsoleRenderer::ScreenDrawUIFromFile(&m_fgameDialog[m_igameDialogIndex].m_fDialogue, m_fgameDialog[m_igameDialogIndex].m_fDialogue.m_iUIColor);
}