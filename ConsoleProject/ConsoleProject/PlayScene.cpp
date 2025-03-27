#include "PlayScene.h"
#include "Input.h"
#include "Game.h"
#include "UI.h"
#include "Object.h"

// Timer 관련
float m_fcurrentTimePlayScene = 0;
float m_floadingTimePlayScene = 1/60;
float m_fMenuLastTimePlayScene = 0;

const char* m_speechNextString = ">>>";
const char* m_speechNextStringBlank = "   ";

UI::FUI m_fSpeechSlate;
UI::FUI m_fSpeechNextCursor;
float m_fspeechNextCountOneSecond = 0;

UI::FUI m_fAsciiCatIdlePlaySceneFile;
UI::FUI m_fAsciiCatEyeOpenPlaySceneFile;
UI::FUI* AsciiArt_cat_PlayScene = NULL;
bool bcatBlinkPlayScene = false;

UI::FGAMEDIALOG m_fgameDialog[MAX_DIALOG_SIZE];
int m_fgameDialogSize = 0;
int m_fSpeechContentIndex = 1;

struct PlaySceneObj
{
	UI::FUI CatIdle;
	UI::FUI CatCurios;
	UI::FUI CatEyeOpen;
	UI::FUI CatEyeOpen_TailDown;
	UI::FUI CatFace;
	UI::FUI CatJump;
	UI::FUI CatOnRoof;
	UI::FUI CatScared;
	UI::FUI CatWalking;
	UI::FUI CatWeird;

	UI::FUI BirdAttack;
	UI::FUI BirdIdle;
};

PlaySceneObj m_fplaySceneobj;
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

/// <summary>
/// / 플레이어
/// </summary>
struct PlayScenePlayerObj
{
	UI::FUI Player1Idle;
	UI::FUI Player1Blink;
	UI::FUI Player2Idle;
	UI::FUI Player2Blink;
	UI::FUI Player3Idle;
	UI::FUI Player3Blink;
};

PlayScenePlayerObj m_fpossiblePlayerCharacterobj;
UI::FUI* m_pCurPlayerCharacter = NULL;
UI::FUI m_fCursorPlayScene;

UI::FUI m_fVideoPlayScene[MAX_VIDEO_SIZE];	// 400 * 233
int m_ivideoPlayClipMax = 0;
int m_ivideoPlaycursor = 0;

Object::FPlayerCharacter m_fPlayerCharacter;
Object::FPlayerCharacter m_fEnemyCharacter;
Object::FActor m_fBullet;
Object::FActor m_fCrossHair;
Object::Node* m_pBulletHead = NULL;
float m_fDefaultBulletSpeed = 10.0f;

// Move 관련
int m_iPlayerMoveXAmountPlayScene = 3;
int m_iPlayerMoveYAmountPlayScene = 2;
int m_iCrossHairMoveXAmountPlayScene = 7;
int m_iCrossHairMoveYAmountPlayScene = 5;

void PlayScene::Initialize()	// 게임 시작할 때 초기화
{
	Object::SetPlayerAnimationName(&m_fPlayerCharacter, (char*)"RapiFullBody", (char*)"RapiCover", (char*)"RapiAim", (char*)"RapiAimFire");
	Object::LoadAnimationData(&m_fPlayerCharacter);
	m_fPlayerCharacter.m_eAnimationState = Object::EAnimationState::COVER;
	m_fPlayerCharacter.m_bPlayable = true;
	m_fPlayerCharacter.m_fAxis.X = ConsoleRenderer::ScreenCenter(m_fPlayerCharacter.m_fanimation[m_fPlayerCharacter.m_eAnimationState].m_fui->m_ppcontent[0]);
	m_fPlayerCharacter.m_fAxis.Y = ConsoleRenderer::ScreenHeight() * 0.62;


	Object::SetPlayerAnimationName(&m_fEnemyCharacter, (char*)"ModerniaFullBody");
	Object::LoadAnimationData(&m_fEnemyCharacter);
	m_fEnemyCharacter.m_eAnimationState = Object::EAnimationState::FULLBODY_IDLE;
	m_fEnemyCharacter.m_bPlayable = true;
	m_fEnemyCharacter.m_fAxis.X = ConsoleRenderer::ScreenCenter(m_fEnemyCharacter.m_fanimation[m_fEnemyCharacter.m_eAnimationState].m_fui->m_ppcontent[0]);
	m_fEnemyCharacter.m_fAxis.Y = 0 ;


	m_fSpeechContentIndex = 1;

	m_fCursorPlayScene = UI::FUI(int(ConsoleRenderer::ScreenWidth() * 0.4), int(ConsoleRenderer::ScreenHeight() * 0.7), (char*)" > ");

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

	m_fAsciiCatIdlePlaySceneFile.m_fAxis.X = ConsoleRenderer::ScreenWidth() * 0.1;
	m_fAsciiCatIdlePlaySceneFile.m_fAxis.Y = ConsoleRenderer::ScreenHeight() * 0.1;

	m_fAsciiCatEyeOpenPlaySceneFile.m_fAxis.X = ConsoleRenderer::ScreenWidth() * 0.1;
	m_fAsciiCatEyeOpenPlaySceneFile.m_fAxis.Y = ConsoleRenderer::ScreenHeight() * 0.1;

	// 타이머 초기화
	m_fcurrentTimePlayScene = Time::GetTotalTime();
}

void PlayScene::LoadData()
{
	for (int i = 0; i < MAX_VIDEO_SIZE; i++)
	{
		char filename[256];
		sprintf_s(filename, sizeof(filename), "Video/Aru-frame_%04d.txt", i+1);
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
	else
	{
		m_fBullet.m_fAxis.X = ConsoleRenderer::ScreenWidth() * 0.2;
		m_fBullet.m_fAxis.Y = ConsoleRenderer::ScreenHeight()/ 2;
	}

	if (FileController::FileRead("CrossHair.txt", "r", &m_fCrossHair.m_fui.m_ppcontent, &m_fCrossHair.m_fui.m_ippcontentSize) == false)
		ConsoleRenderer::print((char*)"PlayScene_FileReadError CatIdle\n");
	else
	{
		m_fPlayerCharacter.m_fAxis.X = ConsoleRenderer::ScreenCenter(m_fCrossHair.m_fui.m_ppcontent[0]);
		m_fPlayerCharacter.m_fAxis.Y = ConsoleRenderer::ScreenHeight() * 0.5;
	}

	if (FileController::FileRead("CatIdle.txt", "r", &m_fAsciiCatIdlePlaySceneFile.m_ppcontent, &m_fAsciiCatIdlePlaySceneFile.m_ippcontentSize) == false)
		ConsoleRenderer::print((char*)"PlayScene_FileReadError CatIdle\n");
	else
		AsciiArt_cat_PlayScene = &m_fAsciiCatIdlePlaySceneFile;

	if (FileController::FileRead("CatEyeOpen.txt", "r", &m_fAsciiCatEyeOpenPlaySceneFile.m_ppcontent, &m_fAsciiCatEyeOpenPlaySceneFile.m_ippcontentSize) == false)
		ConsoleRenderer::print((char*)"MenuScene_FileReadError\n");

	if (FileController::FileRead("CatCurios.txt", "r", &m_fplaySceneobj.CatCurios.m_ppcontent, &m_fplaySceneobj.CatCurios.m_ippcontentSize) == false)
		ConsoleRenderer::print((char*)"MenuScene_FileReadError CatCurios \n");
	else
	{
		m_fplaySceneobj.CatCurios.m_fAxis.X = ConsoleRenderer::ScreenWidth() * PlaySceneMagnitudeX - strlen(m_fplaySceneobj.CatCurios.m_ppcontent[0]) / 2;
		m_fplaySceneobj.CatCurios.m_fAxis.Y = ConsoleRenderer::ScreenHeight() * 0.5f - m_fplaySceneobj.CatCurios.m_ippcontentSize / 2;
	}

	if (FileController::FileRead("CatEyeOpen.txt", "r", &m_fplaySceneobj.CatEyeOpen.m_ppcontent, &m_fplaySceneobj.CatEyeOpen.m_ippcontentSize) == false)
		ConsoleRenderer::print((char*)"MenuScene_FileReadError CatCurios \n");
	else
	{
		m_fplaySceneobj.CatEyeOpen.m_fAxis.X = ConsoleRenderer::ScreenWidth() * PlaySceneMagnitudeX - strlen(m_fplaySceneobj.CatEyeOpen.m_ppcontent[0]) / 2;
		m_fplaySceneobj.CatEyeOpen.m_fAxis.Y = ConsoleRenderer::ScreenHeight() * 0.5f - m_fplaySceneobj.CatCurios.m_ippcontentSize / 2;
	}

	if (FileController::FileRead("CatEyeOpen_TailDown.txt", "r", &m_fplaySceneobj.CatEyeOpen_TailDown.m_ppcontent, &m_fplaySceneobj.CatEyeOpen_TailDown.m_ippcontentSize) == false)
		ConsoleRenderer::print((char*)"MenuScene_FileReadError CatCurios \n");
	else
	{
		m_fplaySceneobj.CatEyeOpen_TailDown.m_fAxis.X = ConsoleRenderer::ScreenWidth() * PlaySceneMagnitudeX - strlen(m_fplaySceneobj.CatEyeOpen_TailDown.m_ppcontent[0]) / 2;
		m_fplaySceneobj.CatEyeOpen_TailDown.m_fAxis.Y = ConsoleRenderer::ScreenHeight() * 0.5f - m_fplaySceneobj.CatCurios.m_ippcontentSize / 2;
	}

	if (FileController::FileRead("CatFace.txt", "r", &m_fplaySceneobj.CatFace.m_ppcontent, &m_fplaySceneobj.CatFace.m_ippcontentSize) == false)
		ConsoleRenderer::print((char*)"MenuScene_FileReadError CatCurios \n");
	else
	{
		m_fplaySceneobj.CatFace.m_fAxis.X = ConsoleRenderer::ScreenWidth() * PlaySceneMagnitudeX - strlen(m_fplaySceneobj.CatFace.m_ppcontent[0]) / 2;
		m_fplaySceneobj.CatFace.m_fAxis.Y = ConsoleRenderer::ScreenHeight() * 0.3f - m_fplaySceneobj.CatCurios.m_ippcontentSize / 2;
	}

	if (FileController::FileRead("CatIdle.txt", "r", &m_fplaySceneobj.CatIdle.m_ppcontent, &m_fplaySceneobj.CatIdle.m_ippcontentSize) == false)
		ConsoleRenderer::print((char*)"MenuScene_FileReadError CatCurios \n");
	else
	{
		m_fplaySceneobj.CatIdle.m_fAxis.X = ConsoleRenderer::ScreenWidth() * PlaySceneMagnitudeX - strlen(m_fplaySceneobj.CatIdle.m_ppcontent[0]) / 2;
		m_fplaySceneobj.CatIdle.m_fAxis.Y = ConsoleRenderer::ScreenHeight() * 0.5f - m_fplaySceneobj.CatCurios.m_ippcontentSize / 2;
	}

	if (FileController::FileRead("CatJump.txt", "r", &m_fplaySceneobj.CatJump.m_ppcontent, &m_fplaySceneobj.CatJump.m_ippcontentSize) == false)
		ConsoleRenderer::print((char*)"MenuScene_FileReadError CatCurios \n");
	else
	{
		m_fplaySceneobj.CatJump.m_fAxis.X = ConsoleRenderer::ScreenWidth() * PlaySceneMagnitudeX - strlen(m_fplaySceneobj.CatJump.m_ppcontent[0]) / 2;
		m_fplaySceneobj.CatJump.m_fAxis.Y = ConsoleRenderer::ScreenHeight() * 0.1f - m_fplaySceneobj.CatCurios.m_ippcontentSize / 2;
	}

	if (FileController::FileRead("CatOnRoof.txt", "r", &m_fplaySceneobj.CatOnRoof.m_ppcontent, &m_fplaySceneobj.CatOnRoof.m_ippcontentSize) == false)
		ConsoleRenderer::print((char*)"MenuScene_FileReadError CatCurios \n");
	else
	{
		m_fplaySceneobj.CatOnRoof.m_fAxis.X = ConsoleRenderer::ScreenWidth() * PlaySceneMagnitudeX - strlen(m_fplaySceneobj.CatOnRoof.m_ppcontent[0]) / 2;
		m_fplaySceneobj.CatOnRoof.m_fAxis.Y = ConsoleRenderer::ScreenHeight() * 0.4f - m_fplaySceneobj.CatCurios.m_ippcontentSize / 2;
	}

	if (FileController::FileRead("CatScared.txt", "r", &m_fplaySceneobj.CatScared.m_ppcontent, &m_fplaySceneobj.CatScared.m_ippcontentSize) == false)
		ConsoleRenderer::print((char*)"MenuScene_FileReadError CatCurios \n");
	else
	{
		m_fplaySceneobj.CatScared.m_fAxis.X = ConsoleRenderer::ScreenWidth() * PlaySceneMagnitudeX - strlen(m_fplaySceneobj.CatScared.m_ppcontent[0]) / 2;
		m_fplaySceneobj.CatScared.m_fAxis.Y = ConsoleRenderer::ScreenHeight() * 0.5f - m_fplaySceneobj.CatCurios.m_ippcontentSize / 2;
	}
	if (FileController::FileRead("CatWalking.txt", "r", &m_fplaySceneobj.CatWalking.m_ppcontent, &m_fplaySceneobj.CatWalking.m_ippcontentSize) == false)
		ConsoleRenderer::print((char*)"MenuScene_FileReadError CatCurios \n");
	else
	{
		m_fplaySceneobj.CatWalking.m_fAxis.X = ConsoleRenderer::ScreenWidth() * 0.4f - strlen(m_fplaySceneobj.CatWalking.m_ppcontent[0]) / 2;
		m_fplaySceneobj.CatWalking.m_fAxis.Y = ConsoleRenderer::ScreenHeight() * 0.05f - m_fplaySceneobj.CatCurios.m_ippcontentSize / 2;
	}

	if (FileController::FileRead("CatWeird.txt", "r", &m_fplaySceneobj.CatWeird.m_ppcontent, &m_fplaySceneobj.CatWeird.m_ippcontentSize) == false)
		ConsoleRenderer::print((char*)"MenuScene_FileReadError CatCurios \n");
	else
	{
		m_fplaySceneobj.CatWeird.m_fAxis.X = ConsoleRenderer::ScreenWidth() * 0.3f - strlen(m_fplaySceneobj.CatWeird.m_ppcontent[0]) / 2;
		m_fplaySceneobj.CatWeird.m_fAxis.Y = ConsoleRenderer::ScreenHeight() * 0.1f - m_fplaySceneobj.CatCurios.m_ippcontentSize / 2;
	}

	if (FileController::FileRead("BirdIdle.txt", "r", &m_fplaySceneobj.BirdIdle.m_ppcontent, &m_fplaySceneobj.BirdIdle.m_ippcontentSize) == false)
		ConsoleRenderer::print((char*)"MenuScene_FileReadError CatCurios \n");
	else
	{
		m_fplaySceneobj.BirdIdle.m_fAxis.X = ConsoleRenderer::ScreenWidth() * 0.4f - strlen(m_fplaySceneobj.BirdIdle.m_ppcontent[0]) / 2;
		m_fplaySceneobj.BirdIdle.m_fAxis.Y = ConsoleRenderer::ScreenHeight() * -0.1f - m_fplaySceneobj.CatCurios.m_ippcontentSize / 2;
	}

	if (FileController::FileRead("BirdAttack.txt", "r", &m_fplaySceneobj.BirdAttack.m_ppcontent, &m_fplaySceneobj.BirdAttack.m_ippcontentSize) == false)
		ConsoleRenderer::print((char*)"MenuScene_FileReadError CatCurios \n");
	else
	{
		m_fplaySceneobj.BirdAttack.m_fAxis.X = ConsoleRenderer::ScreenWidth() * 0.4f - strlen(m_fplaySceneobj.BirdAttack.m_ppcontent[0]) / 2;
		m_fplaySceneobj.BirdAttack.m_fAxis.Y = ConsoleRenderer::ScreenHeight() * -0.1f - m_fplaySceneobj.CatCurios.m_ippcontentSize / 2;
	}

	if (FileController::FileRead("Player1Idle.txt", "r", &m_fpossiblePlayerCharacterobj.Player1Idle.m_ppcontent, &m_fpossiblePlayerCharacterobj.Player1Idle.m_ippcontentSize) == false)
		ConsoleRenderer::print((char*)"MenuScene_FileReadError CatCurios \n");
	else
	{
		m_fpossiblePlayerCharacterobj.Player1Idle.m_fAxis.X = ConsoleRenderer::ScreenWidth() * 0.4f - strlen(m_fpossiblePlayerCharacterobj.Player1Idle.m_ppcontent[0]) / 2;
		m_fpossiblePlayerCharacterobj.Player1Idle.m_fAxis.Y = ConsoleRenderer::ScreenHeight() * -0.1f - m_fpossiblePlayerCharacterobj.Player1Idle.m_ippcontentSize / 2;
	}

	if (FileController::FileRead("Player1Idle.txt", "r", &m_fpossiblePlayerCharacterobj.Player1Blink.m_ppcontent, &m_fpossiblePlayerCharacterobj.Player1Blink.m_ippcontentSize) == false)
		ConsoleRenderer::print((char*)"MenuScene_FileReadError CatCurios \n");
	else
	{
		m_fpossiblePlayerCharacterobj.Player1Blink.m_fAxis.X = ConsoleRenderer::ScreenWidth() * 0.4f - strlen(m_fpossiblePlayerCharacterobj.Player1Blink.m_ppcontent[0]) / 2;
		m_fpossiblePlayerCharacterobj.Player1Blink.m_fAxis.Y = ConsoleRenderer::ScreenHeight() * -0.1f - m_fpossiblePlayerCharacterobj.Player1Blink.m_ippcontentSize / 2;
	}

	if (FileController::FileRead("Player1Idle.txt", "r", &m_fpossiblePlayerCharacterobj.Player2Idle.m_ppcontent, &m_fpossiblePlayerCharacterobj.Player2Idle.m_ippcontentSize) == false)
		ConsoleRenderer::print((char*)"MenuScene_FileReadError CatCurios \n");
	else
	{
		m_fpossiblePlayerCharacterobj.Player2Idle.m_fAxis.X = ConsoleRenderer::ScreenWidth() * 0.4f - strlen(m_fpossiblePlayerCharacterobj.Player2Idle.m_ppcontent[0]) / 2;
		m_fpossiblePlayerCharacterobj.Player2Idle.m_fAxis.Y = ConsoleRenderer::ScreenHeight() * -0.1f - m_fpossiblePlayerCharacterobj.Player2Idle.m_ippcontentSize / 2;
	}

	if (FileController::FileRead("Player1Idle.txt", "r", &m_fpossiblePlayerCharacterobj.Player2Blink.m_ppcontent, &m_fpossiblePlayerCharacterobj.Player2Blink.m_ippcontentSize) == false)
		ConsoleRenderer::print((char*)"MenuScene_FileReadError CatCurios \n");
	else
	{
		m_fpossiblePlayerCharacterobj.Player2Blink.m_fAxis.X = ConsoleRenderer::ScreenWidth() * 0.4f - strlen(m_fpossiblePlayerCharacterobj.Player2Blink.m_ppcontent[0]) / 2;
		m_fpossiblePlayerCharacterobj.Player2Blink.m_fAxis.Y = ConsoleRenderer::ScreenHeight() * -0.1f - m_fpossiblePlayerCharacterobj.Player2Blink.m_ippcontentSize / 2;
	}

	if (FileController::FileRead("Player3Idle.txt", "r", &m_fpossiblePlayerCharacterobj.Player3Idle.m_ppcontent, &m_fpossiblePlayerCharacterobj.Player3Idle.m_ippcontentSize) == false)
		ConsoleRenderer::print((char*)"MenuScene_FileReadError CatCurios \n");
	else
	{
		m_fpossiblePlayerCharacterobj.Player3Idle.m_fAxis.X = ConsoleRenderer::ScreenWidth() * 0.4f - strlen(m_fpossiblePlayerCharacterobj.Player3Idle.m_ppcontent[0]) / 2;
		m_fpossiblePlayerCharacterobj.Player3Idle.m_fAxis.Y = ConsoleRenderer::ScreenHeight() * -0.1f - m_fpossiblePlayerCharacterobj.Player3Idle.m_ippcontentSize / 2;
	}

	if (FileController::FileRead("Player3Idle.txt", "r", &m_fpossiblePlayerCharacterobj.Player3Blink.m_ppcontent, &m_fpossiblePlayerCharacterobj.Player3Blink.m_ippcontentSize) == false)
		ConsoleRenderer::print((char*)"MenuScene_FileReadError CatCurios \n");
	else
	{
		m_fpossiblePlayerCharacterobj.Player3Blink.m_fAxis.X = ConsoleRenderer::ScreenWidth() * 0.4f - strlen(m_fpossiblePlayerCharacterobj.Player3Blink.m_ppcontent[0]) / 2;
		m_fpossiblePlayerCharacterobj.Player3Blink.m_fAxis.Y = ConsoleRenderer::ScreenHeight() * -0.1f - m_fpossiblePlayerCharacterobj.Player3Blink.m_ippcontentSize / 2;
	}
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

	if (Input::IsKeyPressed(VK_SPACE) || Input::IsKeyPressed(VK_RETURN))
	{
		m_fSpeechContentIndex = (m_fSpeechContentIndex + 1) % m_fgameDialogSize;
	}

	if (Input::IsKeyPressed(VK_BACK))
	{
		m_fSpeechContentIndex - 1 < 0 ? m_fSpeechContentIndex = m_fgameDialogSize - 1 : m_fSpeechContentIndex = (m_fSpeechContentIndex - 1);
	}
	
	if (Input::IsKeyDown(VK_RIGHT))
	{
		//m_fPlayerCharacter.m_fAxis.X += m_iPlayerMoveXAmountPlayScene;
		m_fCrossHair.m_fAxis.X += m_iCrossHairMoveXAmountPlayScene;

		//m_fAsciiCatIdlePlaySceneFile.m_fAxis.X += m_iPlayerMoveXAmountPlayScene;
		//m_fAsciiCatEyeOpenPlaySceneFile.m_fAxis.X += m_iPlayerMoveXAmountPlayScene;
	}
	if (Input::IsKeyDown(VK_LEFT))
	{
		m_fCrossHair.m_fAxis.X -= m_iCrossHairMoveXAmountPlayScene;
		//m_fPlayerCharacter.m_fAxis.X -= m_iPlayerMoveXAmountPlayScene;

		//m_fAsciiCatIdlePlaySceneFile.m_fAxis.X -= m_iPlayerMoveXAmountPlayScene;
		//m_fAsciiCatEyeOpenPlaySceneFile.m_fAxis.X -= m_iPlayerMoveXAmountPlayScene;
	}
	if (Input::IsKeyDown(VK_UP))
	{
		//m_fPlayerCharacter.m_fAxis.Y -= m_iPlayerMoveYAmountPlayScene;
		m_fCrossHair.m_fAxis.Y -= m_iCrossHairMoveYAmountPlayScene;

		//m_fAsciiCatIdlePlaySceneFile.m_fAxis.Y -= m_iPlayerMoveYAmountPlayScene;
		//m_fAsciiCatEyeOpenPlaySceneFile.m_fAxis.Y -= m_iPlayerMoveYAmountPlayScene;
	}
	if (Input::IsKeyDown(VK_DOWN))
	{
		//m_fPlayerCharacter.m_fAxis.Y += m_iPlayerMoveYAmountPlayScene;
		m_fCrossHair.m_fAxis.Y += m_iCrossHairMoveYAmountPlayScene;

		//m_fAsciiCatIdlePlaySceneFile.m_fAxis.Y += m_iPlayerMoveYAmountPlayScene;
		//m_fAsciiCatEyeOpenPlaySceneFile.m_fAxis.Y += m_iPlayerMoveYAmountPlayScene;
	}

	if (Input::IsKeyPressed(VK_T))
	{
		int fontsize = ConsoleRenderer::GetScreenFontSize();
		ConsoleRenderer::SetScreenFontSize(fontsize + 1);
		ConsoleRenderer::ScreenInit();
	}

	if (Input::IsKeyPressed(VK_R))
	{
		int fontsize = ConsoleRenderer::GetScreenFontSize();
		ConsoleRenderer::SetScreenFontSize(fontsize - 1);
		ConsoleRenderer::ScreenInit();
	}

	if (Input::IsKeyPressed(VK_E))
	{
		m_fPlayerCharacter.m_bPlayable = !m_fPlayerCharacter.m_bPlayable;
	}

	if (Input::IsKeyDown(VK_RIGHT) || Input::IsKeyDown(VK_LEFT) || Input::IsKeyDown(VK_UP) || Input::IsKeyDown(VK_DOWN))
	{
		if (Input::IsKeyDown(VK_SPACE))
		{
			m_fPlayerCharacter.m_eAnimationState = Object::EAnimationState::AIMFIRE;
		}
		else
		{
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
		m_fPlayerCharacter.m_eAnimationState = Object::EAnimationState::AIMFIRE;
		Object::FActor Actor = Object::FActor(m_fCrossHair.m_fAxis.X + strlen(m_fCrossHair.m_fui.m_ppcontent[0]) / 2, m_fCrossHair.m_fAxis.Y + m_fCrossHair.m_fui.m_ippcontentSize / 2, m_fBullet);

 		float xAxis = m_fCrossHair.m_fAxis.X - Actor.m_fAxis.X;
		float yAxis = m_fCrossHair.m_fAxis.Y - Actor.m_fAxis.Y;
		float Magnitude = sqrt(powf(xAxis, 2) + powf(yAxis, 2));
		COORD dirVector = { xAxis / Magnitude * 100, yAxis / Magnitude * 100};
		m_pBulletHead = Object::Add(m_pBulletHead, Actor, dirVector, m_fDefaultBulletSpeed);
	}
}

void PlayScene::Release()
{
	UI::Release(&m_fSpeechSlate);
	UI::Release(&m_fAsciiCatIdlePlaySceneFile);
	UI::Release(&m_fAsciiCatEyeOpenPlaySceneFile);
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
}

void PlayScene::Update()
{
	Input::Update();
	PlayScene::ProcessInput();

	m_fMenuLastTimePlayScene = Time::GetTotalTime() - m_fcurrentTimePlayScene;
	if (m_fMenuLastTimePlayScene >= m_floadingTimePlayScene)
	{
		m_fcurrentTimePlayScene = Time::GetTotalTime();

		if (m_fPlayerCharacter.m_bPlayable)
			m_fPlayerCharacter.m_iPlaybackCurrentSeconds = (m_fPlayerCharacter.m_iPlaybackCurrentSeconds + 1) % m_fPlayerCharacter.m_fanimation[m_fPlayerCharacter.m_eAnimationState].m_iMaxLength;

		if (m_fEnemyCharacter.m_bPlayable)
			m_fEnemyCharacter.m_iPlaybackCurrentSeconds = (m_fEnemyCharacter.m_iPlaybackCurrentSeconds + 1) % m_fEnemyCharacter.m_fanimation[m_fEnemyCharacter.m_eAnimationState].m_iMaxLength;
		//m_ivideoPlaycursor = (m_ivideoPlaycursor + 1 ) % m_ivideoPlayClipMax;
	}

	if (m_fSpeechContentIndex == 0)
	{
		//Game::ChangeScene(ESceneState::END);
	}
}

void PlayScene::Render()
{
	ConsoleRenderer::ScreenDrawUI(&m_fCursorPlayScene, FG_WHITE);
	switch (m_eplaySceneState)
	{
	case DIALOGUE:
		ConsoleRenderer::ScreenDrawUIFromFile(&m_fSpeechSlate, FG_SKY);
		ConsoleRenderer::ScreenDrawUI(&m_fSpeechNextCursor, FG_WHITE);
		BlinkSpeechNextButton();
		SpeechContent();
		CutScene();
		ConsoleRenderer::ScreenDrawUIFromFile(AsciiArt_cat_PlayScene, FG_WHITE);
		break;
	case SHOOTING:
		ConsoleRenderer::ScreenDrawUIFromFile(AsciiArt_cat_PlayScene, FG_WHITE);
		break;
	case RHYTHM:
		break;
	case PUZZLE:
		break;
	case ANIMATION:
		//ConsoleRenderer::ScreenDrawUIFromFile(&m_fVideoPlayScene[m_ivideoPlaycursor], FG_WHITE);
		//ConsoleRenderer::ScreenDrawUIFromFile(&PlayerCharacter.m_fanimation[PlayerCharacter.m_eAnimationState].m_fui[PlayerCharacter.m_iPlaybackCurrentSeconds], FG_WHITE);
		//ConsoleRenderer::ScreenDrawUIFromFile(&PlayerCharacter.m_fanimation[1].m_fui[0], FG_WHITE);
		ConsoleRenderer::ScreenDrawPlayerWithAnimation(m_fEnemyCharacter.m_fAxis.X, m_fEnemyCharacter.m_fAxis.Y, &m_fEnemyCharacter.m_fanimation[m_fEnemyCharacter.m_eAnimationState].m_fui[m_fEnemyCharacter.m_iPlaybackCurrentSeconds], FG_WHITE);
		ConsoleRenderer::ScreenDrawPlayerWithAnimation(m_fPlayerCharacter.m_fAxis.X, m_fPlayerCharacter.m_fAxis.Y, &m_fPlayerCharacter.m_fanimation[m_fPlayerCharacter.m_eAnimationState].m_fui[m_fPlayerCharacter.m_iPlaybackCurrentSeconds],FG_WHITE);
		break;
	default:
		break;
	}

	ConsoleRenderer::ScreenDrawFileStrings(m_fBullet.m_fAxis.X, m_fBullet.m_fAxis.Y, m_fBullet.m_fui.m_ppcontent, m_fBullet.m_fui.m_ippcontentSize, FG_WHITE);
	ConsoleRenderer::ScreenDrawFileStrings(m_fCrossHair.m_fAxis.X, m_fCrossHair.m_fAxis.Y, m_fCrossHair.m_fui.m_ppcontent, m_fCrossHair.m_fui.m_ippcontentSize, FG_WHITE);
	Object::RenderAllNode(m_pBulletHead);
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

void PlayScene::CutScene()
{
	if (strcmp(m_fgameDialog[m_fSpeechContentIndex].SceneName.m_pcontent, "CatIdle") == false)
		ConsoleRenderer::ScreenDrawUIFromFile(&m_fplaySceneobj.CatIdle, FG_WHITE);
	else if (strcmp(m_fgameDialog[m_fSpeechContentIndex].SceneName.m_pcontent, "CatCurios") == false)
		ConsoleRenderer::ScreenDrawUIFromFile(&m_fplaySceneobj.CatCurios, FG_WHITE);
	else if (strcmp(m_fgameDialog[m_fSpeechContentIndex].SceneName.m_pcontent, "CatEyeOpen") == false)
		ConsoleRenderer::ScreenDrawUIFromFile(&m_fplaySceneobj.CatEyeOpen, FG_WHITE);
	else if (strcmp(m_fgameDialog[m_fSpeechContentIndex].SceneName.m_pcontent, "CatEyeOpen_TailDown") == false)
		ConsoleRenderer::ScreenDrawUIFromFile(&m_fplaySceneobj.CatEyeOpen_TailDown, FG_WHITE);
	else if (strcmp(m_fgameDialog[m_fSpeechContentIndex].SceneName.m_pcontent, "CatFace") == false)
		ConsoleRenderer::ScreenDrawUIFromFile(&m_fplaySceneobj.CatFace, FG_WHITE);
	else if (strcmp(m_fgameDialog[m_fSpeechContentIndex].SceneName.m_pcontent, "CatJump") == false)
		ConsoleRenderer::ScreenDrawUIFromFile(&m_fplaySceneobj.CatJump, FG_WHITE);
	else if (strcmp(m_fgameDialog[m_fSpeechContentIndex].SceneName.m_pcontent, "CatOnRoof") == false)
		ConsoleRenderer::ScreenDrawUIFromFile(&m_fplaySceneobj.CatOnRoof, FG_WHITE);
	else if (strcmp(m_fgameDialog[m_fSpeechContentIndex].SceneName.m_pcontent, "CatScared") == false)
		ConsoleRenderer::ScreenDrawUIFromFile(&m_fplaySceneobj.CatScared, FG_WHITE);
	else if (strcmp(m_fgameDialog[m_fSpeechContentIndex].SceneName.m_pcontent, "CatWalking") == false)
		ConsoleRenderer::ScreenDrawUIFromFile(&m_fplaySceneobj.CatWalking, FG_WHITE);
	else if (strcmp(m_fgameDialog[m_fSpeechContentIndex].SceneName.m_pcontent, "CatWeird") == false)
		ConsoleRenderer::ScreenDrawUIFromFile(&m_fplaySceneobj.CatWeird, FG_WHITE);
	else if (strcmp(m_fgameDialog[m_fSpeechContentIndex].SceneName.m_pcontent, "BirdAttack") == false)
		ConsoleRenderer::ScreenDrawUIFromFile(&m_fplaySceneobj.BirdAttack, FG_WHITE);
	else if (strcmp(m_fgameDialog[m_fSpeechContentIndex].SceneName.m_pcontent, "BirdIdle") == false)
		ConsoleRenderer::ScreenDrawUIFromFile(&m_fplaySceneobj.BirdIdle, FG_WHITE);
}