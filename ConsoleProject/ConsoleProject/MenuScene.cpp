#include "MenuScene.h"
#include "AsciiArt.h"
#include "Input.h"
#include "Game.h"

float m_fcurrentTime = 0;
float m_floadingTime = 5;
float m_fMenuLastTime = 0;

void MenuScene::Initialize()	// 게임 시작할 때 초기화
{
	if (Game::GetTimer())
		m_fcurrentTime = Game::GetTimer()->GetTotalTime();
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
	}
}

void MenuScene::Render()
{
	const char* Title = u8"MenuScene !!!";
	ConsoleRenderer::ScreenDrawString(ConsoleRenderer::ScreenCenter(Title), 0, Title, FG_YELLOW);

	const char* year2025 = AsciiArt::Getyear2025();
	ConsoleRenderer::ScreenDrawMultilineString(ConsoleRenderer::ScreenCenter(Title), 5, year2025, BG_BLACK);

	LoadingBar(m_fMenuLastTime / m_floadingTime * 100);

	const char* AsciiArt_Aru = AsciiArt::Aru;
	ConsoleRenderer::ScreenDrawMultilineString(0, 15, AsciiArt_Aru, FG_WHITE);

	//const char* AsciiArt_Monariza = AsciiArt::Monariza;
	//ConsoleRenderer::ScreenDrawMultilineString(0, 15, AsciiArt_Monariza, FG_WHITE);

	/*const char* AsciiArt_Lion = AsciiArt::GetLion();
	ConsoleRenderer::ScreenDrawMultilineString(0, 2, AsciiArt_Lion, BG_WHITE);*/

	/*const char* girlAsciiArt = AsciiArt::GetSunGlass();
	ConsoleRenderer::ScreenDrawMultilineString(2, 2, girlAsciiArt, BG_WHITE);*/

	//ConsoleRenderer::ScreenDrawString(2, 2, u8"▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄", BG_WHITE);
	//ConsoleRenderer::ScreenDrawString(2, 3, u8"█░░░░░░░░▀█▄▀▄▀██████░▀█▄▀▄▀██████░", BG_WHITE);
	//ConsoleRenderer::ScreenDrawString(2, 4, u8"░░░░░░░░░░░▀█▄█▄███▀░░░ ▀██▄█▄███▀░", BG_WHITE);

	//int x = 10, y = 6;
	//ConsoleRenderer::ScreenDrawString(x, y + 0, "  (^_^)", FG_WHITE);
	//ConsoleRenderer::ScreenDrawString(x, y + 1, "  <| |>", FG_WHITE);
	//ConsoleRenderer::ScreenDrawString(x, y + 2, "  /  \\ ", FG_WHITE);

	//ConsoleRenderer::ScreenDrawChar(g_Player.X, g_Player.Y, 'P', FG_WHITE);

}

void MenuScene::LoadingBar(float value)
{
	const char* message = nullptr;
	if (value >= 100.0f)
		message = AsciiArt::Loading100percent;
	else if (value >= 90.0f)
		message = AsciiArt::Loading90percent;
	else if (value >= 80.0f)
		message = AsciiArt::Loading80percent;
	else if (value >= 70.0f)
		message = AsciiArt::Loading70percent;
	else if (value >= 60.0f)
		message = AsciiArt::Loading60percent;
	else if (value >= 50.0f)
		message = AsciiArt::Loading50percent;
	else if (value >= 40.0f)
		message = AsciiArt::Loading40percent;
	else if (value >= 30.0f)
		message = AsciiArt::Loading30percent;
	else if (value >= 20.0f)
		message = AsciiArt::Loading20percent;
	else if (value >= 10.0f)
		message = AsciiArt::Loading10percent;
	else if (value >= 1.0f)
		message = AsciiArt::Loading1percent;
	else
		message = AsciiArt::LoadingProcess;

	// 출력 위치는 예시이며 필요에 따라 조정 가능
	ConsoleRenderer::ScreenDrawString(ConsoleRenderer::ScreenCenter(AsciiArt::LoadingTitle), 1, AsciiArt::LoadingTitle, FG_WHITE);
	ConsoleRenderer::ScreenDrawString(ConsoleRenderer::ScreenWidth() / 2, 3, message, FG_WHITE);
}
