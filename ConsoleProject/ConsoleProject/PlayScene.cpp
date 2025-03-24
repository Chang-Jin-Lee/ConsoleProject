#include "PlayScene.h"
#include "Input.h"
#include "Game.h"

#define MAX_DIALOG_SIZE 100

// Timer 관련
int m_icatMoveXAmountPlayScene = 3;
int m_icatMoveYAmountPlayScene = 2;
float m_fcurrentTimePlayScene = 0;
float m_floadingTimePlayScene = 0.5;
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
int m_fSpeechContentIndex = 0;

void PlayScene::Initialize()	// 게임 시작할 때 초기화
{
	LoadData();

	int SpeechSlateYSize = ConsoleRenderer::ScreenHeight() * 0.3;
	int SpeechSlateXSize = ConsoleRenderer::ScreenWidth();
	m_fSpeechSlate.m_ppcontent = (char**)malloc( sizeof(char*) * SpeechSlateYSize);

	for (int i = 0; i < SpeechSlateYSize; i++)
		m_fSpeechSlate.m_ppcontent[i] = (char*)malloc(sizeof(char) * SpeechSlateXSize);

	m_fSpeechSlate.m_icontentSize = SpeechSlateYSize;
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
	}

	m_fAsciiCatIdlePlaySceneFile.m_fAxis.X = ConsoleRenderer::ScreenWidth() * 0.1;
	m_fAsciiCatIdlePlaySceneFile.m_fAxis.Y = ConsoleRenderer::ScreenHeight() * 0.1;

	m_fAsciiCatEyeOpenPlaySceneFile.m_fAxis.X = ConsoleRenderer::ScreenWidth() * 0.1;
	m_fAsciiCatEyeOpenPlaySceneFile.m_fAxis.Y = ConsoleRenderer::ScreenHeight() * 0.1;

	// 타이머 초기화
	m_fcurrentTimePlayScene = Game::GetTimer()->GetTotalTime();
}

void PlayScene::LoadData()
{
	if (FileController::FileReadFromCSV("Dialogue.csv", "r", m_fgameDialog, &m_fgameDialogSize) == false)
		ConsoleRenderer::print((char*)"PlayScene_FileReadError\n");

	if (FileController::FileRead("CatIdle.txt", "r", &m_fAsciiCatIdlePlaySceneFile.m_ppcontent, &m_fAsciiCatIdlePlaySceneFile.m_icontentSize) == false)
		ConsoleRenderer::print((char*)"PlayScene_FileReadError CatIdle\n");
	else
		AsciiArt_cat_PlayScene = &m_fAsciiCatIdlePlaySceneFile;

	if (FileController::FileRead("CatEyeOpen.txt", "r", &m_fAsciiCatEyeOpenPlaySceneFile.m_ppcontent, &m_fAsciiCatEyeOpenPlaySceneFile.m_icontentSize) == false)
		ConsoleRenderer::print((char*)"MenuScene_FileReadError\n");
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

	if (Input::IsKeyPressed(VK_RIGHT))
	{
		m_fAsciiCatIdlePlaySceneFile.m_fAxis.X += m_icatMoveXAmountPlayScene;
		m_fAsciiCatEyeOpenPlaySceneFile.m_fAxis.X += m_icatMoveXAmountPlayScene;
	}
	if (Input::IsKeyPressed(VK_LEFT))
	{
		m_fAsciiCatIdlePlaySceneFile.m_fAxis.X -= m_icatMoveXAmountPlayScene;
		m_fAsciiCatEyeOpenPlaySceneFile.m_fAxis.X -= m_icatMoveXAmountPlayScene;
	}
	if (Input::IsKeyPressed(VK_UP))
	{
		m_fAsciiCatIdlePlaySceneFile.m_fAxis.Y -= m_icatMoveYAmountPlayScene;
		m_fAsciiCatEyeOpenPlaySceneFile.m_fAxis.Y -= m_icatMoveYAmountPlayScene;
	}
	if (Input::IsKeyPressed(VK_DOWN))
	{
		m_fAsciiCatIdlePlaySceneFile.m_fAxis.Y += m_icatMoveYAmountPlayScene;
		m_fAsciiCatEyeOpenPlaySceneFile.m_fAxis.Y += m_icatMoveYAmountPlayScene;
	}
}

void PlayScene::Update()
{
	Input::Update();
	PlayScene::ProcessInput();

	m_fMenuLastTimePlayScene = Game::GetTimer()->GetTotalTime() - m_fcurrentTimePlayScene;
	if (m_fMenuLastTimePlayScene >= m_floadingTimePlayScene)
	{
		//Game::ChangeScene(ESceneState::PLAY);
		bcatBlinkPlayScene = !bcatBlinkPlayScene;
		bcatBlinkPlayScene == true ? AsciiArt_cat_PlayScene = &m_fAsciiCatEyeOpenPlaySceneFile : AsciiArt_cat_PlayScene = &m_fAsciiCatIdlePlaySceneFile;
		m_fcurrentTimePlayScene = Game::GetTimer()->GetTotalTime();
	}
}

void PlayScene::Render()
{
	ConsoleRenderer::ScreenDrawUIFromFile(m_fSpeechSlate, FG_WHITE);
	ConsoleRenderer::ScreenDrawUI(m_fSpeechNextCursor, FG_WHITE);

	BlinkSpeechNextButton();
	SpeechContent();

	ConsoleRenderer::ScreenDrawUIFromFile(*AsciiArt_cat_PlayScene, FG_WHITE);
}
void PlayScene::BlinkSpeechNextButton()
{
	if (Game::GetTimer()->GetTotalTime() - m_fspeechNextCountOneSecond >= 0.7)	// 1초에 한 번씩
	{
		m_fSpeechNextCursor.m_pcontent == (char*)m_speechNextString ? m_fSpeechNextCursor.m_pcontent = (char*)m_speechNextStringBlank : m_fSpeechNextCursor.m_pcontent = (char*)m_speechNextString;
		m_fspeechNextCountOneSecond = Game::GetTimer()->GetTotalTime();
	}
}

void PlayScene::SpeechContent()
{
	ConsoleRenderer::ScreenDrawUI(m_fgameDialog[m_fSpeechContentIndex].Speaker, FG_WHITE);
	ConsoleRenderer::ScreenDrawUI(m_fgameDialog[m_fSpeechContentIndex].Dialog, FG_WHITE);
	ConsoleRenderer::ScreenDrawUI(m_fgameDialog[m_fSpeechContentIndex].Type, FG_WHITE);
}