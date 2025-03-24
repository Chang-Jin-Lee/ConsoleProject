#include "PlayScene.h"
#include "Input.h"
#include "Game.h"

#define MAX_SPEECH_SIZE 100

const char* m_speechNextString = ">>>";
const char* m_speechNextStringBlank = "   ";

UI::FUI m_fSpeechSlate;
UI::FUI m_fSpeechNextCursor;
float m_fspeechNextCountOneSecond = 0;

UI::FUI m_fSpeechContents[MAX_SPEECH_SIZE];
int m_fSpeechContentIndex = 0;

void PlayScene::Initialize()	// 게임 시작할 때 초기화
{
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

	for (int i = 0; i < MAX_SPEECH_SIZE; i++)
	{
		m_fSpeechContents[i].m_fAxis.X = m_fSpeechSlate.m_fAxis.X + ConsoleRenderer::ScreenWidth() * 0.15;
		m_fSpeechContents[i].m_fAxis.Y = m_fSpeechSlate.m_fAxis.Y + SpeechSlateYSize / 2;

		m_fSpeechContents[i].m_pcontent = (char*)"testtestsetsetest";
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

	if (Input::IsKeyReleased(VK_RIGHT))
	{
		m_fSpeechContentIndex++;
	}
}

void PlayScene::Update()
{
	Input::Update();
	PlayScene::ProcessInput();

	BlinkSpeechNextButton();
}

void PlayScene::Render()
{
	ConsoleRenderer::ScreenDrawUIFromFile(m_fSpeechSlate, FG_WHITE);
	ConsoleRenderer::ScreenDrawUI(m_fSpeechNextCursor, FG_WHITE);
	ConsoleRenderer::ScreenDrawUI(m_fSpeechContents[m_fSpeechContentIndex], FG_WHITE);
}
void PlayScene::BlinkSpeechNextButton()
{
	if (Game::GetTimer()->GetTotalTime() - m_fspeechNextCountOneSecond >= 0.7)	// 1초에 한 번씩
	{
		m_fSpeechNextCursor.m_pcontent == (char*)m_speechNextString ? m_fSpeechNextCursor.m_pcontent = (char*)m_speechNextStringBlank : m_fSpeechNextCursor.m_pcontent = (char*)m_speechNextString;
		m_fspeechNextCountOneSecond = Game::GetTimer()->GetTotalTime();
	}
}