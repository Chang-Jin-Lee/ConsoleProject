// ConsoleDoubleBuffering.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#ifdef MEMORY_LEAK_CHECK
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include <iostream>
#include <Windows.h>
#include <conio.h>
#include "Game.h"
#include "AnimationScene.h"
#include "GameOverScene.h"

ESceneState g_eSceneCurrentState = MENU;
ESceneState g_eSceneNextState = MENU;
COORD g_Player = { 0,0 };
bool g_bQuit = false;

int wmain()
{
#ifdef MEMORY_LEAK_CHECK
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	Time::Initialize();
	Game::Initialize();

	while (!g_bQuit)
	{
		Game::Update();	// g_eSceneCurrentState에 따라서 Update() 계속 실행.
		Game::Render();
	};

	Game::Release();
#ifdef MEMORY_LEAK_CHECK
	_CrtDumpMemoryLeaks();
#endif
}

void Game::Initialize()	// 게임 시작할 때 초기화
{
	ConsoleRenderer::ScreenClear();
	ConsoleRenderer::ScreenInit();

	Game::LoadData();
	switch (g_eSceneCurrentState)
	{
	case MENU:
		MenuScene::Initialize();
		break;
	case ANIMATION:
		AnimationScene::Initialize();
		break;
	case PLAY:
		PlayScene::Initialize();
		break;
	case END:
		EndScene::Initialize();
		break;
	case GAMEOVER:
		GameOverScene::Initialize();
		break;
	default:
		break;
	}
	ConsoleRenderer::ScreenInit();
}

void Game::LoadData()
{
	switch (g_eSceneCurrentState)
	{
	case MENU:
		MenuScene::LoadData();
		break;
	case ANIMATION:
		AnimationScene::LoadData();
		break;
	case PLAY:
		PlayScene::LoadData();
		break;
	case END:
		EndScene::LoadData();
		break;
	case GAMEOVER:
		GameOverScene::LoadData();
		break;
	default:
		break;
	}
}

void Game::Update()
{
	Time::UpdateTime();
	switch (g_eSceneCurrentState)
	{
	case MENU:
		MenuScene::Update();
		break;
	case ANIMATION:
		AnimationScene::Update();
		break;
	case PLAY:
		PlayScene::Update();
		break;
	case END:
		EndScene::Update();
		break;
	case GAMEOVER:
		GameOverScene::Update();
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
	case MENU:
		MenuScene::Render();
		break;
	case ANIMATION:
		AnimationScene::Render();
		break;
	case PLAY:
		PlayScene::Render();
		break;
	case END:
		EndScene::Render();
		break;
	case GAMEOVER:
		GameOverScene::Render();
		break;
	default:
		break;
	}

	ConsoleRenderer::ScreenFlipping();
}

void Game::Release()
{
	switch (g_eSceneCurrentState)
	{
	case MENU:
		MenuScene::Release();
		break;
	case ANIMATION:
		AnimationScene::Release();
		break;
	case PLAY:
		PlayScene::Release();
		break;
	case END:
		EndScene::Release();
		break;
	case GAMEOVER:
		GameOverScene::Release();
		break;
	default:
		break;
	}
	ConsoleRenderer::ScreenRelease();
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
		Game::Release();
		g_eSceneCurrentState = g_eSceneNextState;
		Game::Initialize();
	}
}

void Game::GameExit()
{
	g_bQuit = true;
}
