#include "MenuScene.h"
#include "AsciiArt.h"
#include "Input.h"
#include "Game.h"

#define MAX_PERCENT_STRING_SIZE 40

float m_fcurrentTime = 0;
float m_floadingTime = 5;
float m_fMenuLastTime = 0;

float m_finitialOneSecond = 0;
float m_fcountOneSeconds = 0;
float m_fPlayer_x = 0;
float m_fPlayer_y = 0;

char** m_pAlpha = NULL;
int m_pAlpha_size = 0;

char** m_ptitle = NULL;
int m_ptitle_size = 0;

wchar_t* m_pPercent = NULL;

void MenuScene::Initialize()	// 게임 시작할 때 초기화
{
	if (Game::GetTimer())
	{
		m_fcurrentTime = Game::GetTimer()->GetTotalTime();
		m_finitialOneSecond = Game::GetTimer()->GetTotalTime();
	}

	if (FileController::FileRead("MenuTitle.txt", "r", &m_ptitle, &m_ptitle_size) == false)
	{
		printf("FileReadError\n");
	}


	if (FileController::FileRead("Rupi.txt", "r", &m_pAlpha, &m_pAlpha_size) == false)
	{
		printf("FileReadError\n");
	}

	m_pPercent = (wchar_t*)malloc(sizeof(wchar_t)* MAX_PERCENT_STRING_SIZE);
	for (int i = 0; i < MAX_PERCENT_STRING_SIZE; i++)
		m_pPercent[i] = AsciiArt::blankFill;
}

void MenuScene::ProcessInput()
{
	if (Input::IsKeyPressed(VK_Q))
	{ 
		Game::ChangeScene(ESceneState::PLAY);
	}

	if (Input::IsKeyPressed(VK_ESCAPE)) { //종료
		Game::GameExit();
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
			Game::ChangeScene(ESceneState::PLAY);
		}

		m_fcountOneSeconds = Game::GetTimer()->GetTotalTime() - m_finitialOneSecond;
		if (m_fcountOneSeconds >= 0.5)
		{
			m_finitialOneSecond = Game::GetTimer()->GetTotalTime();
			m_fPlayer_x += 3;
			//m_fPlayer_y -= 10;
			m_fcountOneSeconds = 0;
		}
	}
}

void MenuScene::Render()
{
	ConsoleRenderer::ScreenDrawFileStrings(ConsoleRenderer::ScreenCenter(m_ptitle[0]), ConsoleRenderer::ScreenHeight() * 0.1, m_ptitle, m_ptitle_size, FG_WHITE);
	LoadingBar(m_fMenuLastTime / m_floadingTime);


	//const char* Title = u8"MenuScene !!!";
	//ConsoleRenderer::ScreenDrawString(ConsoleRenderer::ScreenCenter(Title), 0, Title, FG_YELLOW);
	//const char* year2025 = AsciiArt::Getyear2025();
	//ConsoleRenderer::ScreenDrawMultilineString(ConsoleRenderer::ScreenCenter(Title), 5, year2025, BG_BLACK);
	//ConsoleRenderer::ScreenDrawFileStrings(m_fPlayer_x, 10, m_pAlpha, m_pAlpha_size, FG_WHITE);
}

void MenuScene::LoadingBar(float value)
{
	//const char* message = nullptr;
	wchar_t message[MAX_PERCENT_STRING_SIZE];
	int messageSize = min(MAX_PERCENT_STRING_SIZE-1, MAX_PERCENT_STRING_SIZE * value);

	wcsncpy_s(message, m_pPercent, messageSize);

	char valueToString[5];
	_itoa_s(min(100,100 * value), valueToString,10);

	// 출력 위치는 예시이며 필요에 따라 조정 가능
	ConsoleRenderer::ScreenDrawString(ConsoleRenderer::ScreenCenter(valueToString), ConsoleRenderer::ScreenHeight() * 0.5, valueToString, FG_WHITE);
	ConsoleRenderer::ScreenDrawWString(ConsoleRenderer::ScreenCenterW(m_pPercent), ConsoleRenderer::ScreenHeight() * 0.4, message, FG_WHITE);
}
