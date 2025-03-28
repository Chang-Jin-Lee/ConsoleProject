#include "PlayScene.h"
#include "Input.h"
#include "Game.h"
#include "UI.h"
#include "Object.h"

// Timer 관련
float m_fcurrentTimePlayScene = 0;
float m_fAnimationTimePlayScene = 1/60;
float m_fMenuLastTimePlayScene = 0;

const char* m_speechNextString = ">>>";
const char* m_speechNextStringBlank = "   ";

UI::FUI m_fSpeechSlate;
UI::FUI m_fSpeechNextCursor;
float m_fspeechNextCountOneSecond = 0;

UI::FGAMEDIALOG m_fgameDialog[MAX_DIALOG_SIZE];
int m_fgameDialogSize = 0;
int m_fSpeechContentIndex = 1;

UI::PlaySceneObj m_fplaySceneobj;
float PlaySceneMagnitudeX = 0.5;

enum EPlaySceneState
{
	DIALOGUE,
	SHOOTING,
	RHYTHM,
	PUZZLE,
	ANIMATION,
};

EPlaySceneState m_eplaySceneState = ANIMATION;

UI::PlayScenePlayerObj m_fpossiblePlayerCharacterobj;
UI::FUI* m_pCurPlayerCharacter = NULL;
UI::FUI m_fSpeechCursorPlayScene;

UI::FUI m_fVideoPlayScene[MAX_VIDEO_SIZE];	// 400 * 233
int m_ivideoPlayClipMax = 0;
int m_ivideoPlaycursor = 0;

bool bIsReloading = false;
Object::FPlayerCharacter m_fPlayerCharacter;
Object::FPlayerCharacter m_fEnemyCharacter;
Object::FActor m_fBullet;
Object::FCrossHair m_fCrossHair;
Object::Node* m_pBulletHead = NULL;
float m_fDefaultBulletSpeed = 10.0f;

// Move 관련
int m_iPlayerMoveXAmountPlayScene = 3;
int m_iPlayerMoveYAmountPlayScene = 2;
int m_iCrossHairMoveXAmountPlayScene = 10;
int m_iCrossHairMoveYAmountPlayScene = 5;

const float m_fcrossHairSizeRatio = 1.3f;
bool bCrossHairMove = false;
int m_icrossHairTargetXSize = 0;
int m_icrossHairOriginalXSize = 0;
int m_icrossHairTargetYSize = 0;
int m_icrossHairOriginalYSize = 0;

void PlayScene::Initialize()	// 게임 시작할 때 초기화
{
	// Initialize Player
	Object::SetPlayerAnimationName(&m_fPlayerCharacter, (char*)"RapiFullBody", (char*)"RapiCover", (char*)"RapiAim", (char*)"RapiAimFire", (char*)"RapiReload");
	Object::LoadAnimationData(&m_fPlayerCharacter);
	m_fPlayerCharacter.m_eAnimationState = Object::EAnimationState::COVER;
	m_fPlayerCharacter.m_bPlayable = true;
	m_fPlayerCharacter.m_fAxis.X = ConsoleRenderer::ScreenCenter(m_fPlayerCharacter.m_fanimation[m_fPlayerCharacter.m_eAnimationState].m_fui->m_ppcontent[0]);
	m_fPlayerCharacter.m_fAxis.Y = ConsoleRenderer::ScreenHeight() * 0.52;
	Object::CreateAndAttachHealthBar(&m_fPlayerCharacter, COORD{ 0,SHORT(ConsoleRenderer::ScreenHeight() * 0.005) }, FG_GREEN);
	m_fPlayerCharacter.m_iColor = FG_WHITE;
	m_fPlayerCharacter.m_iHP = 100;
	m_fPlayerCharacter.m_iAmmo = 30;
	m_fPlayerCharacter.m_iMaxAmmo = 30;
	m_fPlayerCharacter.m_iFireDamage = 2;

	// Initialize Enemy 
	Object::SetPlayerAnimationName(&m_fEnemyCharacter, (char*)"ModerniaFullBody");
	Object::LoadAnimationData(&m_fEnemyCharacter);
	m_fEnemyCharacter.m_eAnimationState = Object::EAnimationState::FULLBODY_IDLE;
	m_fEnemyCharacter.m_bPlayable = true;
	m_fEnemyCharacter.m_fAxis.X = ConsoleRenderer::ScreenCenter(m_fEnemyCharacter.m_fanimation[m_fEnemyCharacter.m_eAnimationState].m_fui->m_ppcontent[0]);
	m_fEnemyCharacter.m_fAxis.Y = -ConsoleRenderer::ScreenHeight() * 0.1;
	Object::CreateAndAttachHealthBar(&m_fEnemyCharacter, COORD{ 0,SHORT(ConsoleRenderer::ScreenHeight() * 0.12) }, FG_RED);
	m_fEnemyCharacter.m_iColor= FG_WHITE;
	m_fEnemyCharacter.m_iHP = 500;
	m_fEnemyCharacter.m_iAmmo = 30;
	m_fEnemyCharacter.m_iMaxAmmo = 30;
	m_fEnemyCharacter.m_iFireDamage = 10;

	// Initialize Crosshair
	m_fCrossHair.m_fAxis.X = ConsoleRenderer::ScreenHeight() * 0.5;
	m_fCrossHair.m_fAxis.Y = ConsoleRenderer::ScreenHeight() * 0.5;
	m_fCrossHair.m_fAspectRatio = 0.012;
	Object::CreateCrossHair(&m_fCrossHair);
	m_icrossHairOriginalXSize = m_fCrossHair.m_sXDistanceFromCenter;
	m_icrossHairOriginalYSize = m_fCrossHair.m_sYDistanceFromCenter;

	m_icrossHairTargetXSize = m_fCrossHair.m_sXDistanceFromCenter * m_fcrossHairSizeRatio;
	m_icrossHairTargetYSize = m_fCrossHair.m_sYDistanceFromCenter * m_fcrossHairSizeRatio;
	m_fCrossHair.m_iColor = FG_RED;
	bCrossHairMove = false;

	// Initialize Bullet
	m_fBullet.m_fAxis.X = ConsoleRenderer::ScreenWidth() * 0.2;
	m_fBullet.m_fAxis.Y = ConsoleRenderer::ScreenHeight() / 2;
	m_fBullet.m_iColor = FG_BLUE;

	// Initialize Speech 
	m_fSpeechContentIndex = 1;
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
	}

	// Initialize Timer
	m_fcurrentTimePlayScene = Time::GetTotalTime();
}

void PlayScene::LoadData()	// 각 애니메이션에 대한 데이터를 읽어온다
{
	for (int i = 0; i < MAX_VIDEO_SIZE; i++)
	{
		char filename[256];
		sprintf_s(filename, sizeof(filename), "Video/Aru/frame_%04d.txt", i+1);
		if (FileController::FileRead(filename, "r", &m_fVideoPlayScene[i].m_ppcontent, &m_fVideoPlayScene[i].m_ippcontentSize) == false)
		{
			m_ivideoPlayClipMax = i;
			ConsoleRenderer::print((char*)"PlayScene_FileReadError m_fVideoPlayScene ");
			ConsoleRenderer::print(filename);
			ConsoleRenderer::print((char*)"\n");
			break;
		}
	}

	if (FileController::FileReadFromCSV("Dialogue.csv", "r", m_fgameDialog, &m_fgameDialogSize) == false)
		ConsoleRenderer::print((char*)"PlayScene_FileReadError\n");

	if (FileController::FileRead("Bullet.txt", "r", &m_fBullet.m_fui.m_ppcontent, &m_fBullet.m_fui.m_ippcontentSize) == false)
		ConsoleRenderer::print((char*)"PlayScene_FileReadError CatIdle\n");
}

void PlayScene::ProcessInput()
{
	if (Input::IsKeyPressed(VK_Q))
	{
		Game::ChangeScene(ESceneState::END);
	}

	if (Input::IsKeyPressed(VK_ESCAPE)) //종료
	{
		Game::GameExit();
	}

	if (Input::IsKeyPressed(VK_OEM_6)) // ]
	{
		m_iCrossHairMoveXAmountPlayScene *= 1.5;
		m_iCrossHairMoveYAmountPlayScene *= 1.5;
	}

	if (Input::IsKeyPressed(VK_OEM_4)) // [
	{
		m_iCrossHairMoveXAmountPlayScene /= 1.5;
		m_iCrossHairMoveYAmountPlayScene /= 1.5;
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

	if (Input::IsKeyPressed(VK_SPACE) || Input::IsKeyPressed(VK_RETURN))
	{
		m_fSpeechContentIndex = (m_fSpeechContentIndex + 1) % m_fgameDialogSize;
	}

	if (Input::IsKeyPressed(VK_BACK))
	{
		m_fSpeechContentIndex - 1 < 0 ? m_fSpeechContentIndex = m_fgameDialogSize - 1 : m_fSpeechContentIndex = (m_fSpeechContentIndex - 1);
	}

	if (Input::IsKeyPressed(VK_R))
	{
		m_fPlayerCharacter.m_eAnimationState = Object::EAnimationState::RELOAD;
		bIsReloading = true;
	}

	if (bIsReloading) return;

	if (Input::IsKeyPressed(VK_E))
	{
		m_fPlayerCharacter.m_bPlayable = !m_fPlayerCharacter.m_bPlayable;
	}

	if (Input::IsKeyDown(VK_RIGHT) || Input::IsKeyDown(VK_LEFT) || Input::IsKeyDown(VK_UP) || Input::IsKeyDown(VK_DOWN))
	{
		if (Input::IsKeyDown(VK_RIGHT))
		{
			m_fCrossHair.m_fAxis.X += m_iCrossHairMoveXAmountPlayScene;
		}
		if (Input::IsKeyDown(VK_LEFT))
		{
			m_fCrossHair.m_fAxis.X -= m_iCrossHairMoveXAmountPlayScene;
		}
		if (Input::IsKeyDown(VK_UP))
		{
			m_fCrossHair.m_fAxis.Y -= m_iCrossHairMoveYAmountPlayScene;
		}
		if (Input::IsKeyDown(VK_DOWN))
		{
			m_fCrossHair.m_fAxis.Y += m_iCrossHairMoveYAmountPlayScene;
		}

		if (Input::IsKeyDown(VK_SPACE))
		{
			bCrossHairMove = true;
			m_fPlayerCharacter.m_eAnimationState = Object::EAnimationState::AIMFIRE;
		}
		else
		{
			bCrossHairMove = false;
			m_fPlayerCharacter.m_eAnimationState = Object::EAnimationState::AIM;
		}
	}

	if (!Input::IsKeyDown(VK_RIGHT) && !Input::IsKeyDown(VK_LEFT) && !Input::IsKeyDown(VK_UP) && !Input::IsKeyDown(VK_DOWN) && !Input::IsKeyDown(VK_SPACE))
	{
		m_fPlayerCharacter.m_eAnimationState = Object::EAnimationState::COVER;
	}

	if (Input::IsKeyPressed(VK_Z))
	{
		m_fPlayerCharacter.m_eAnimationState = Object::EAnimationState::COVER;
	}
	if (Input::IsKeyPressed(VK_X))
	{
		m_fPlayerCharacter.m_eAnimationState = Object::EAnimationState::AIM;
	}
	if (Input::IsKeyPressed(VK_C))
	{
		m_fPlayerCharacter.m_eAnimationState = Object::EAnimationState::COVER;
	}

	if (Input::IsKeyDown(VK_SPACE))
	{
		bCrossHairMove = true;
		m_fPlayerCharacter.m_eAnimationState = Object::EAnimationState::AIMFIRE;

		// Effect Spawn

		// Bullet Spawn
		//Object::FActor Actor = Object::FActor(m_fCrossHair.m_fAxis.X + strlen(m_fCrossHair.m_fui.m_ppcontent[0]) / 2, m_fCrossHair.m_fAxis.Y + m_fCrossHair.m_fui.m_ippcontentSize / 2, m_fBullet);
		//Object::FActor Actor = Object::FActor( ConsoleRenderer::ScreenWidth() / 2, m_fPlayerCharacter.m_fAxis.Y, m_fBullet);
 		//float xAxis = m_fCrossHair.m_fAxis.X - Actor.m_fAxis.X;
		//float yAxis = m_fCrossHair.m_fAxis.Y - Actor.m_fAxis.Y;
		//float Magnitude = sqrt(powf(xAxis, 2) + powf(yAxis, 2));
		//COORD dirVector = { xAxis / Magnitude * 100, yAxis / Magnitude * 100};
		//COORD DestinationVector = { m_fCrossHair.m_fAxis.X, m_fCrossHair.m_fAxis.Y };
		//m_pBulletHead = Object::Add(m_pBulletHead, Actor, dirVector, DestinationVector, m_fDefaultBulletSpeed);
	}
	else
	{
		bCrossHairMove = false;
	}
}

void PlayScene::Release()
{
	UI::Release(&m_fSpeechSlate);
	for (int i = 0; i < MAX_DIALOG_SIZE; i++)
	{
		UI::Release(&m_fgameDialog[i].Speaker);
		UI::Release(&m_fgameDialog[i].Answer);
		UI::Release(&m_fgameDialog[i].Dialog);
		UI::Release(&m_fgameDialog[i].Likeability);
		UI::Release(&m_fgameDialog[i].SceneName);
		UI::Release(&m_fgameDialog[i].Type);
	}
	UI::Release(&m_fplaySceneobj.CatIdle);
	UI::Release(&m_fplaySceneobj.CatCurios);
	UI::Release(&m_fplaySceneobj.CatEyeOpen);
	UI::Release(&m_fplaySceneobj.CatEyeOpen_TailDown);
	UI::Release(&m_fplaySceneobj.CatFace);
	UI::Release(&m_fplaySceneobj.CatJump);
	UI::Release(&m_fplaySceneobj.CatOnRoof);
	UI::Release(&m_fplaySceneobj.CatScared);
	UI::Release(&m_fplaySceneobj.CatWalking);
	UI::Release(&m_fplaySceneobj.CatWeird);
	UI::Release(&m_fplaySceneobj.BirdAttack);
	UI::Release(&m_fplaySceneobj.BirdIdle);

	Object::Release(&m_fPlayerCharacter);
	Object::Release(&m_fEnemyCharacter);
	Object::Release(&m_fCrossHair.Top);
	Object::Release(&m_fCrossHair.Left);
	Object::Release(&m_fCrossHair.Right);
	Object::Release(&m_fCrossHair.Bottom);
}

void PlayScene::Update()
{
	Input::Update();
	PlayScene::ProcessInput();

	m_fMenuLastTimePlayScene = Time::GetTotalTime() - m_fcurrentTimePlayScene;
	if (m_fMenuLastTimePlayScene >= m_fAnimationTimePlayScene)
	{
		m_fcurrentTimePlayScene = Time::GetTotalTime();

		if (m_fPlayerCharacter.m_bPlayable)
		{
			if (bIsReloading && m_fPlayerCharacter.m_eAnimationState == Object::EAnimationState::RELOAD)
			{
				if (m_fPlayerCharacter.m_iPlaybackCurrentSeconds >= m_fPlayerCharacter.m_fanimation[m_fPlayerCharacter.m_eAnimationState].m_iMaxLength - 1)
				{
					bIsReloading = false;
				}
			}
			m_fPlayerCharacter.m_iPlaybackCurrentSeconds = (m_fPlayerCharacter.m_iPlaybackCurrentSeconds + 1) % m_fPlayerCharacter.m_fanimation[m_fPlayerCharacter.m_eAnimationState].m_iMaxLength;
		}

		if (m_fEnemyCharacter.m_bPlayable)
			m_fEnemyCharacter.m_iPlaybackCurrentSeconds = (m_fEnemyCharacter.m_iPlaybackCurrentSeconds + 1) % m_fEnemyCharacter.m_fanimation[m_fEnemyCharacter.m_eAnimationState].m_iMaxLength;
	}

	Object::UpdateAllNodeAxis(m_pBulletHead, Time::GetElapsedTime());
	if (bCrossHairMove)
	{
		if(m_fCrossHair.m_sXDistanceFromCenter < m_icrossHairTargetXSize)
			Object::UpdateCrossHairIncreaseSize(&m_fCrossHair, Time::GetElapsedTime());
		else
			Object::UpdateCrossHairDecreaseSize(&m_fCrossHair, Time::GetElapsedTime());
	}
}

void PlayScene::Render()
{
	switch (m_eplaySceneState)
	{
	case DIALOGUE:
		ConsoleRenderer::ScreenDrawUIFromFile(&m_fSpeechSlate, FG_SKY);
		ConsoleRenderer::ScreenDrawUI(&m_fSpeechNextCursor, FG_WHITE);
		//BlinkSpeechNextButton();
		//SpeechContent();
		break;
	case SHOOTING:
		break;
	case RHYTHM:
		break;
	case PUZZLE:
		break;
	case ANIMATION:
		//ConsoleRenderer::ScreenDrawUIFromFile(&m_fVideoPlayScene[m_ivideoPlaycursor], FG_WHITE);
		//ConsoleRenderer::ScreenDrawUIFromFile(&PlayerCharacter.m_fanimation[PlayerCharacter.m_eAnimationState].m_fui[PlayerCharacter.m_iPlaybackCurrentSeconds], FG_WHITE);
		//ConsoleRenderer::ScreenDrawUIFromFile(&PlayerCharacter.m_fanimation[1].m_fui[0], FG_WHITE);
		ConsoleRenderer::ScreenDrawPlayerWithAnimation(m_fEnemyCharacter.m_fAxis.X, m_fEnemyCharacter.m_fAxis.Y, &m_fEnemyCharacter.m_fanimation[m_fEnemyCharacter.m_eAnimationState].m_fui[m_fEnemyCharacter.m_iPlaybackCurrentSeconds], &m_fEnemyCharacter.m_fHealthBar,  m_fEnemyCharacter.m_iColor);
		ConsoleRenderer::ScreenDrawPlayerWithAnimation(m_fPlayerCharacter.m_fAxis.X, m_fPlayerCharacter.m_fAxis.Y, &m_fPlayerCharacter.m_fanimation[m_fPlayerCharacter.m_eAnimationState].m_fui[m_fPlayerCharacter.m_iPlaybackCurrentSeconds], &m_fPlayerCharacter.m_fHealthBar, m_fPlayerCharacter.m_iColor);
		break;
	default:
		break;
	}

	ConsoleRenderer::ScreenDrawFileStrings(m_fBullet.m_fAxis.X, m_fBullet.m_fAxis.Y, m_fBullet.m_fui.m_ppcontent, m_fBullet.m_fui.m_ippcontentSize, m_fBullet.m_iColor);
	Object::RenderCrossHair(&m_fCrossHair);
	Object::RenderAllNode(m_pBulletHead, m_fBullet.m_iColor);
}
void PlayScene::BlinkSpeechNextButton()
{
	if (Time::GetTotalTime() - m_fspeechNextCountOneSecond >= 0.7)	// 1초에 한 번씩
	{
		m_fSpeechNextCursor.m_pcontent == (char*)m_speechNextString ? m_fSpeechNextCursor.m_pcontent = (char*)m_speechNextStringBlank : m_fSpeechNextCursor.m_pcontent = (char*)m_speechNextString;
		m_fspeechNextCountOneSecond = Time::GetTotalTime();
	}
}

void PlayScene::SpeechContent()
{
	ConsoleRenderer::ScreenDrawUI(&m_fgameDialog[m_fSpeechContentIndex].Speaker, FG_WHITE);
	ConsoleRenderer::ScreenDrawUI(&m_fgameDialog[m_fSpeechContentIndex].Dialog, FG_WHITE);
	ConsoleRenderer::ScreenDrawUI(&m_fgameDialog[m_fSpeechContentIndex].Type, FG_WHITE);
	ConsoleRenderer::ScreenDrawUI(&m_fgameDialog[m_fSpeechContentIndex].Likeability, FG_WHITE);
}