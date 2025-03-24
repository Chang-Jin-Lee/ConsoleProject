#include "MenuScene.h"
#include "Input.h"
#include "Game.h"

#define MAX_PERCENT_STRING_SIZE 40	// 퍼센트 게이지바 칸 개수
#define MAX_BUTTON_COUNT 2			// 메뉴 셀렉트 개수

// Time 관련
float m_fcurrentTime = 0;
float m_floadingTime = 5;
float m_fMenuLastTime = 0;

float m_finitialOneSecond = 0;
float m_fcountOneSeconds = 0;
float m_fPlayer_x = 0;
float m_fPlayer_y = 0;

// cat 모양
float m_fcountOneSecond = 0;
bool bcatBlink = false;

UI::FUI m_fAsciiCatIdleFile;
UI::FUI m_fAsciiCatEyeOpenFile;
UI::FUI* AsciiArt_cat = NULL;

UI::FUI m_fTitleFile;
UI::FUI m_fPercent;
UI::FUI m_fCursor;
UI::FUI m_fStartButton;
UI::FUI m_fExitButton;

enum ECursorState
{
	START, EXIT, MAX
};

int m_apossibleCursorYAxis[MAX_BUTTON_COUNT] = { 0, };	// 버튼들의 개수 만큼 할당.
ECursorState m_ecursorState = START;

void MenuScene::Initialize()	// 게임 시작할 때 초기화
{
	LoadData();

	m_fCursor = UI::FUI(int(ConsoleRenderer::ScreenWidth() * 0.4), int(ConsoleRenderer::ScreenHeight() * 0.7), (char*)" > ");
	m_apossibleCursorYAxis[0] = int(ConsoleRenderer::ScreenHeight() * 0.7);
	m_apossibleCursorYAxis[1] = int(ConsoleRenderer::ScreenHeight() * 0.8);
	m_fStartButton = UI::FUI(int(ConsoleRenderer::ScreenWidth() * 0.45), m_apossibleCursorYAxis[0], (char*)"Game start!");
	m_fExitButton = UI::FUI( int(ConsoleRenderer::ScreenWidth() * 0.45), m_apossibleCursorYAxis[1], (char*)"Game Exit!");

	m_fPercent.m_pcontent = (char*)malloc(sizeof(char) * MAX_PERCENT_STRING_SIZE);
	for (int i = 0; i < MAX_PERCENT_STRING_SIZE; i++)
		m_fPercent.m_pcontent[i] = '-';
	m_fPercent.m_fAxis.X = ConsoleRenderer::ScreenCenter(m_fPercent.m_pcontent);
	m_fPercent.m_fAxis.Y = ConsoleRenderer::ScreenHeight() * 0.4;

	if (Game::GetTimer())
	{
		m_fMenuLastTime = Game::GetTimer()->GetTotalTime();
		m_finitialOneSecond = Game::GetTimer()->GetTotalTime();
		m_fcountOneSecond = Game::GetTimer()->GetTotalTime();
	}
}

void MenuScene::LoadData()
{
	if (FileController::FileRead("MenuTitle.txt", "r", &m_fTitleFile.m_ppcontent, &m_fTitleFile.m_icontentSize))
	{
		m_fTitleFile.m_fAxis.X = ConsoleRenderer::ScreenCenter(m_fTitleFile.m_ppcontent[0]);
		m_fTitleFile.m_fAxis.Y = ConsoleRenderer::ScreenHeight() * 0.1;
	}

	if (FileController::FileRead("CatIdle.txt", "r", &m_fAsciiCatIdleFile.m_ppcontent, &m_fAsciiCatIdleFile.m_icontentSize))
		AsciiArt_cat = &m_fAsciiCatIdleFile;

	if (FileController::FileRead("CatEyeOpen.txt", "r", &m_fAsciiCatEyeOpenFile.m_ppcontent, &m_fAsciiCatEyeOpenFile.m_icontentSize) == false)
		ConsoleRenderer::print((char*)"MenuScene_FileReadError\n");
}

void MenuScene::ProcessInput()
{
	if (Input::IsKeyPressed(VK_ESCAPE))  //종료
	{
		Game::GameExit();
	}

	if (Input::IsKeyPressed(VK_UP)) 
	{ 
		(int)m_ecursorState - 1 < 0 ? m_ecursorState = (ECursorState)(MAX_BUTTON_COUNT - 1) : m_ecursorState = (ECursorState)((int)m_ecursorState -1) ;
		m_fCursor.m_fAxis.Y = m_apossibleCursorYAxis[(int)m_ecursorState];
		bcatBlink = true;
	}
	if (Input::IsKeyReleased(VK_UP))
	{
		bcatBlink = false;
	}

	if (Input::IsKeyPressed(VK_DOWN)) 
	{ 
		m_ecursorState = (ECursorState)(((int)m_ecursorState + 1) % MAX_BUTTON_COUNT);
		m_fCursor.m_fAxis.Y = m_apossibleCursorYAxis[(int)m_ecursorState];
		bcatBlink = true;
	}
	if (Input::IsKeyReleased(VK_DOWN))
	{
		bcatBlink = false;
	}

	if (Input::IsKeyPressed(VK_RETURN) || Input::IsKeyPressed(VK_SPACE))
	{ 
		switch (m_ecursorState)
		{
		case START:
			Game::ChangeScene(ESceneState::PLAY);
			break;
		case EXIT:
			Game::GameExit();
			break;
		case MAX:
			break;
		default:
			break;
		}
	}
}

void MenuScene::Update()
{
	Input::Update();
	MenuScene::ProcessInput();

	if (Game::GetTimer())
	{
		m_fMenuLastTime = Game::GetTimer()->GetTotalTime() - m_fcurrentTime;
		if (m_fMenuLastTime >= m_floadingTime + 1)
		{
			//Game::ChangeScene(ESceneState::PLAY);
		}

		m_fcountOneSeconds = Game::GetTimer()->GetTotalTime() - m_finitialOneSecond;
		if (m_fcountOneSeconds >= 1)
		{
			m_finitialOneSecond = Game::GetTimer()->GetTotalTime();
			m_fPlayer_x += 3;
			//m_fPlayer_y -= 10;
			m_fcountOneSeconds = 0;
		}

		if (Game::GetTimer()->GetTotalTime() - m_fcountOneSecond >= 0.5)	// 0.5초에 한 번씩
		{
			m_fcountOneSecond = Game::GetTimer()->GetTotalTime();
		}

		bcatBlink == true ? AsciiArt_cat = &m_fAsciiCatEyeOpenFile : AsciiArt_cat = &m_fAsciiCatIdleFile;
	}
}

void MenuScene::Render()
{
	TitleText();
	LoadingBar(m_fMenuLastTime / m_floadingTime);
	MenuSlect();
}

void MenuScene::TitleText()
{
	ConsoleRenderer::ScreenDrawUIFromFile(m_fTitleFile, FG_WHITE);
}

void MenuScene::LoadingBar(float value)
{
	char message[MAX_PERCENT_STRING_SIZE];
	int messageSize = min(MAX_PERCENT_STRING_SIZE-1, MAX_PERCENT_STRING_SIZE * value);
	strncpy_s(message, m_fPercent.m_pcontent, messageSize);

	char valueToString[5];
	_itoa_s(min(100, 100 * value), valueToString,10);
}

void MenuScene::MenuSlect()
{
	ConsoleRenderer::ScreenDrawUI(m_fStartButton, FG_WHITE);
	ConsoleRenderer::ScreenDrawUI(m_fExitButton, FG_WHITE);
	ConsoleRenderer::ScreenDrawUI(m_fCursor, FG_WHITE);
	if (AsciiArt_cat)
		ConsoleRenderer::ScreenDrawFileStrings(ConsoleRenderer::ScreenCenter(AsciiArt_cat->m_ppcontent[0]) / 2, ConsoleRenderer::ScreenHeight() * 0.6, AsciiArt_cat->m_ppcontent, AsciiArt_cat->m_icontentSize, FG_WHITE);
}
