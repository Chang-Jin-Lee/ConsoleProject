// ConsoleDoubleBuffering.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

/*
*	Q : MenuScene
*	W : PlayScene
*	E : EndScene
*/

#include <iostream>
#include <Windows.h>
#include <conio.h>
#include "Game.h"

ESceneState g_eSceneCurrentState = ESceneState::MENU;
ESceneState g_eSceneNextState = ESceneState::MENU;
COORD g_Player = { 0,0 };
bool g_bQuit = false;
Time* g_tgameTimer = NULL;

int wmain()
{
	Game::Initialize();

	while (!g_bQuit)
	{
		Game::Update();	// g_eSceneCurrentState에 따라서 Update() 계속 실행.
		Game::Render();
	};

	Game::Release();
}

void Game::Initialize()	// 게임 시작할 때 초기화
{
	Game::LoadData();
	g_tgameTimer = new Time();
	g_tgameTimer->Initialize();
	ConsoleRenderer::ScreenInit();
	Game::SceneInitialize();
}

void Game::LoadData()
{
	switch (g_eSceneCurrentState)
	{
	case ESceneState::MENU:
		MenuScene::LoadData();
		break;
	case ESceneState::PLAY:
		PlayScene::LoadData;
		break;
	case ESceneState::END:
		EndScene::LoadData();
		break;
	default:
		break;
	}
}

void Game::SceneInitialize()
{
	switch (g_eSceneCurrentState)
	{
	case ESceneState::MENU:
		MenuScene::Initialize();
		break;
	case ESceneState::PLAY:
		PlayScene::Initialize();
		break;
	case ESceneState::END:
		EndScene::Initialize();
		break;
	default:
		break;
	}
}

void Game::Update()
{
	Game::GetTimer()->UpdateTime();
	switch (g_eSceneCurrentState)
	{
	case ESceneState::MENU:
		MenuScene::Update();
		break;
	case ESceneState::PLAY:
		PlayScene::Update();
		break;
	case ESceneState::END:
		EndScene::Update();
		break;
	default:
		break;
	}
}

void Game::Render()
{
	ConsoleRenderer::ScreenClear();

	switch (g_eSceneCurrentState)
	{
	case ESceneState::MENU:
		MenuScene::Render();
		break;
	case ESceneState::PLAY:
		PlayScene::Render();
		break;
	case ESceneState::END:
		EndScene::Render();
		break;
	default:
		break;
	}

	ConsoleRenderer::ScreenFlipping();
}

void Game::Release()
{
	delete(g_tgameTimer);
	ConsoleRenderer::ScreenRelease();
}

Time* Game::GetTimer()
{
	return g_tgameTimer;
}

ESceneState* Game::GetCurrentSceneState()
{
	return &g_eSceneCurrentState;
}

void Game::ChangeScene(const ESceneState& scene)
{
	g_eSceneNextState = scene;
	if (g_eSceneNextState != g_eSceneCurrentState)
	{
		g_eSceneCurrentState = g_eSceneNextState;
		Game::SceneInitialize();
	}
}

void Game::GameExit()
{
	g_bQuit = true;
}
