#include "MenuScene.h"
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

// cat 모양
float m_fFPSTimeMenuScene = 1 / 60;
float m_fcountOneSecond = 0;
bool bcatBlink = false;

UI::FUI m_fAsciiCatIdleFile;
UI::FUI m_fAsciiCatEyeOpenFile;
UI::FUI* AsciiArt_cat = NULL;

UI::FUI m_fTitleFile;
UI::FUI m_fCursor;
UI::FUI m_fStartButton;
UI::FUI m_fExitButton;

enum ECursorState
{
	START, EXIT
};

int m_apossibleCursorYAxis[MAX_BUTTON_COUNT] = { 0, };	// 버튼들의 개수 만큼 할당.
ECursorState m_ecursorState = START;

Object::FPlayerCharacter m_fIntroVideo;

void MenuScene::Initialize()	// 게임 시작할 때 초기화
{
	Object::SetPlayerAnimationName(&m_fIntroVideo, (char*)"RapiLobby");
	Object::LoadAnimationData(&m_fIntroVideo);
	m_fIntroVideo.m_eAnimationState = Object::EAnimationState::FULLBODY_IDLE;
	m_fIntroVideo.m_bPlayable = true;
	m_fIntroVideo.m_fAxis.X = ConsoleRenderer::ScreenCenter(m_fIntroVideo.m_fanimation[m_fIntroVideo.m_eAnimationState].m_fui->m_ppcontent[0]);
	m_fIntroVideo.m_fAxis.Y = ConsoleRenderer::ScreenHeight() * 0.1;


	//m_fCursor = UI::FUI(int(ConsoleRenderer::ScreenWidth() * 0.4), int(ConsoleRenderer::ScreenHeight() * 0.7), (char*)" > ");
	//m_apossibleCursorYAxis[0] = int(ConsoleRenderer::ScreenHeight() * 0.7);
	//m_apossibleCursorYAxis[1] = int(ConsoleRenderer::ScreenHeight() * 0.8);
	//m_fStartButton = UI::FUI(int(ConsoleRenderer::ScreenWidth() * 0.45), m_apossibleCursorYAxis[0], (char*)"Game start!");
	//m_fExitButton = UI::FUI(int(ConsoleRenderer::ScreenWidth() * 0.45), m_apossibleCursorYAxis[1], (char*)"Game Exit!");

	m_fMenuLastTime = Time::GetTotalTime();
	m_finitialOneSecond = Time::GetTotalTime();
	m_fcountOneSecond = Time::GetTotalTime();
}

void MenuScene::LoadData()
{
	if (FileController::FileRead("Video/RapiCover/frame_0001.txt", "r", &m_fTitleFile.m_ppcontent, &m_fTitleFile.m_ippcontentSize))
	{
		//m_fTitleFile.m_fAxis.X = ConsoleRenderer::ScreenCenter(m_fTitleFile.m_ppcontent[0]);
		//m_fTitleFile.m_fAxis.Y = ConsoleRenderer::ScreenHeight() * 0.1;
		m_fTitleFile.m_fAxis.X = 0;
		m_fTitleFile.m_fAxis.Y = 0;
	}

	if (FileController::FileRead("CatIdle.txt", "r", &m_fAsciiCatIdleFile.m_ppcontent, &m_fAsciiCatIdleFile.m_ippcontentSize))
		AsciiArt_cat = &m_fAsciiCatIdleFile;

	if (FileController::FileRead("CatEyeOpen.txt", "r", &m_fAsciiCatEyeOpenFile.m_ppcontent, &m_fAsciiCatEyeOpenFile.m_ippcontentSize) == false)
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
		(int)m_ecursorState - 1 < 0 ? m_ecursorState = (ECursorState)(MAX_BUTTON_COUNT - 1) : m_ecursorState = (ECursorState)((int)m_ecursorState - 1);
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
		default:
			break;
		}
	}

	if (Input::IsKeyPressed(VK_T))
	{
		int fontsize = ConsoleRenderer::GetScreenFontSize();
		ConsoleRenderer::SetScreenFontSize(fontsize+1);
		ConsoleRenderer::ScreenInit();

	}

	if (Input::IsKeyPressed(VK_R))
	{
		int fontsize = ConsoleRenderer::GetScreenFontSize();
		ConsoleRenderer::SetScreenFontSize(fontsize - 1);
		ConsoleRenderer::ScreenInit();
	}
}

void MenuScene::Release()
{
	UI::Release(&m_fAsciiCatIdleFile);
	UI::Release(&m_fAsciiCatEyeOpenFile);
	UI::Release(&m_fTitleFile);
}

void MenuScene::Update()
{
	Input::Update();
	MenuScene::ProcessInput();

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
		//m_fPlayer_y -= 10;
		m_fcountOneSeconds = 0;
	}

	if (Time::GetTotalTime() - m_fcountOneSecond >= m_fFPSTimeMenuScene)	// 0.5초에 한 번씩
	{
		if (m_fIntroVideo.m_bPlayable)
		{
			m_fIntroVideo.m_iPlaybackCurrentSeconds = (m_fIntroVideo.m_iPlaybackCurrentSeconds + 1) % m_fIntroVideo.m_fanimation[m_fIntroVideo.m_eAnimationState].m_iMaxLength;
		}
		m_fcountOneSecond = Time::GetTotalTime();
	}

	bcatBlink == true ? AsciiArt_cat = &m_fAsciiCatEyeOpenFile : AsciiArt_cat = &m_fAsciiCatIdleFile;
}

void MenuScene::Render()
{
	TitleText();
	MenuSlect();

	ConsoleRenderer::ScreenDrawPlayerWithAnimation(m_fIntroVideo.m_fAxis.X, m_fIntroVideo.m_fAxis.Y, &m_fIntroVideo.m_fanimation[m_fIntroVideo.m_eAnimationState].m_fui[m_fIntroVideo.m_iPlaybackCurrentSeconds], FG_WHITE);
}

void MenuScene::TitleText()
{
	//ConsoleRenderer::ScreenDrawUIFromFile(&m_fTitleFile, FG_WHITE);
}

void MenuScene::MenuSlect()
{
	//ConsoleRenderer::ScreenDrawUI(&m_fStartButton, FG_WHITE);
	//ConsoleRenderer::ScreenDrawUI(&m_fExitButton, FG_WHITE);
	//ConsoleRenderer::ScreenDrawUI(&m_fCursor, FG_WHITE);
	//if (AsciiArt_cat)
	//	ConsoleRenderer::ScreenDrawFileStrings(ConsoleRenderer::ScreenCenter(AsciiArt_cat->m_ppcontent[0]) / 2, ConsoleRenderer::ScreenHeight() * 0.6, AsciiArt_cat->m_ppcontent, AsciiArt_cat->m_ippcontentSize, FG_WHITE);
}
