#include "PlayScene.h"
#include "Input.h"
#include "Game.h"
#include "UI.h"
#include "Object.h"

// Timer 관련
float m_fcurrentTimePlayScene = 0;
float m_fAnimationTimePlayScene = 1/60;
float m_fMenuLastTimePlayScene = 0;

enum EPlaySceneState
{
	DIALOGUE,
	SHOOTING,
	RHYTHM,
	PUZZLE,
};

EPlaySceneState m_eplaySceneState = SHOOTING;

UI::FUI m_fVideoPlayScene[MAX_VIDEO_SIZE];	// 400 * 233
int m_ivideoPlayClipMax = 0;
int m_ivideoPlaycursor = 0;

bool bIsReloading = false;
Object::FPlayerCharacter m_fPlayerCharacter;
Object::FPlayerCharacter m_fEnemyCharacter;
Object::FLeftAmmoUI m_fLeftAmmoUIPlayScene;
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

// 설명 UI
UI::FUI m_fGuideUIPlayScene;

void PlayScene::Initialize()	// 게임 시작할 때 초기화
{
	// Initialize Player
	m_fPlayerCharacter.m_eAnimationState = Object::EAnimationState::COVER;
	m_fPlayerCharacter.m_bPlayable = true;
	m_fPlayerCharacter.m_fAxis.X = ConsoleRenderer::ScreenCenter(m_fPlayerCharacter.m_fanimation[m_fPlayerCharacter.m_eAnimationState].m_fui->m_ppcontent[0]);
	m_fPlayerCharacter.m_fAxis.Y = (SHORT)(ConsoleRenderer::ScreenHeight() * 0.52);
	Object::CreateAndAttachHealthBar(&m_fPlayerCharacter, COORD{ 0,SHORT(ConsoleRenderer::ScreenHeight() * 0.005) }, FG_GREEN);
	m_fPlayerCharacter.m_iColor = FG_WHITE;
	m_fPlayerCharacter.m_iHealth = 100;
	m_fPlayerCharacter.m_iMaxHealth = 100;
	m_fPlayerCharacter.m_iAmmo = 30;
	m_fPlayerCharacter.m_iMaxAmmo = 30;
	m_fPlayerCharacter.m_iFireDamage = 2;

	// Initialize Enemy 
	m_fEnemyCharacter.m_eAnimationState = Object::EAnimationState::FULLBODYIDLE;
	m_fEnemyCharacter.m_bPlayable = true;
	m_fEnemyCharacter.m_fAxis.X = ConsoleRenderer::ScreenCenter(m_fEnemyCharacter.m_fanimation[m_fEnemyCharacter.m_eAnimationState].m_fui->m_ppcontent[0]);
	m_fEnemyCharacter.m_fAxis.Y = (SHORT)( -ConsoleRenderer::ScreenHeight() * 0.1);
	Object::CreateAndAttachHealthBar(&m_fEnemyCharacter, COORD{ 0,SHORT(ConsoleRenderer::ScreenHeight() * 0.12) }, FG_RED);
	m_fEnemyCharacter.m_iColor= FG_WHITE;
	m_fEnemyCharacter.m_iHealth = 500;
	m_fEnemyCharacter.m_iMaxHealth = 500;
	m_fEnemyCharacter.m_iAmmo = 30;
	m_fEnemyCharacter.m_iMaxAmmo = 30;
	m_fEnemyCharacter.m_iFireDamage = 10;

	// Initialize Crosshair
	m_fCrossHair.m_fAxis.X = (SHORT)(ConsoleRenderer::ScreenHeight() * 0.5);
	m_fCrossHair.m_fAxis.Y = (SHORT)(ConsoleRenderer::ScreenHeight() * 0.5);
	m_fCrossHair.m_fAspectRatio = 0.012f;
	Object::CreateCrossHair(&m_fCrossHair);
	m_icrossHairOriginalXSize = m_fCrossHair.m_sXDistanceFromCenter;
	m_icrossHairOriginalYSize = m_fCrossHair.m_sYDistanceFromCenter;

	m_icrossHairTargetXSize = (int)(m_fCrossHair.m_sXDistanceFromCenter * m_fcrossHairSizeRatio);
	m_icrossHairTargetYSize = (int)(m_fCrossHair.m_sYDistanceFromCenter * m_fcrossHairSizeRatio);
	m_fCrossHair.m_iColor = FG_RED;
	bCrossHairMove = false;

	// Initialize LeftAmmoUI
	m_fLeftAmmoUIPlayScene.m_fAxis.X = (SHORT)(m_fPlayerCharacter.m_fAxis.X - ConsoleRenderer::ScreenWidth() * 0.05);
	m_fLeftAmmoUIPlayScene.m_fAxis.Y = (SHORT)(m_fPlayerCharacter.m_fAxis.Y + ConsoleRenderer::ScreenHeight() * 0.2);
	m_fLeftAmmoUIPlayScene.m_iColor = FG_BLUE;

	// Initialize Timer
	m_fcurrentTimePlayScene = Time::GetTotalTime();


	// Initialize Guide UI
	UI::CreateBubbleUI(&m_fGuideUIPlayScene,
		(int)(ConsoleRenderer::ScreenWidth() * 0.2),
		(int)(ConsoleRenderer::ScreenHeight() * 0.05),
		(int)(m_fLeftAmmoUIPlayScene.m_fAxis.X * 0.5),
		(int)(m_fLeftAmmoUIPlayScene.m_fAxis.Y * 1.1),
		0.02f,
		FG_GREEN
	);
}

void PlayScene::LoadData()	// 각 애니메이션에 대한 데이터를 읽어온다
{
	Object::SetPlayerAnimationName(&m_fPlayerCharacter, (char*)"RapiFullBody", (char*)"RapiCover", (char*)"RapiAim", (char*)"RapiAimFire", (char*)"RapiReload");
	Object::LoadAnimationData(&m_fPlayerCharacter);

	Object::SetPlayerAnimationName(&m_fEnemyCharacter, (char*)"ModerniaFullBody");
	Object::LoadAnimationData(&m_fEnemyCharacter);

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

	int LeftAmmoYSize = (int)(ConsoleRenderer::ScreenHeight() * 0.05);
	int LeftAmmoXSize = (int)(ConsoleRenderer::ScreenWidth() * 0.02);
	m_fLeftAmmoUIPlayScene.m_fui.m_ppcontent = (char**)malloc(sizeof(char*) * LeftAmmoYSize);
	for (int i = 0; i < LeftAmmoYSize; i++)
		m_fLeftAmmoUIPlayScene.m_fui.m_ppcontent[i] = (char*)malloc(sizeof(char) * LeftAmmoXSize);
	m_fLeftAmmoUIPlayScene.m_fui.m_ippcontentSize = LeftAmmoYSize;
	for (int i = 0; i < LeftAmmoYSize; i++)
	{
		for (int j = 0; j < LeftAmmoXSize; j++)
		{
			m_fLeftAmmoUIPlayScene.m_fui.m_ppcontent[i][j] = (char)L'⬛';
		}
		m_fLeftAmmoUIPlayScene.m_fui.m_ppcontent[i][LeftAmmoXSize-1] = '\0';
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

	if (Input::IsKeyPressed(VK_OEM_6)) // ]
	{
		m_iCrossHairMoveXAmountPlayScene = (int)(m_iCrossHairMoveXAmountPlayScene * 1.5);
		m_iCrossHairMoveYAmountPlayScene = (int)(m_iCrossHairMoveYAmountPlayScene * 1.5);
	}

	if (Input::IsKeyPressed(VK_OEM_4)) // [
	{
		m_iCrossHairMoveXAmountPlayScene = (int)(m_iCrossHairMoveXAmountPlayScene / 1.5);
		m_iCrossHairMoveYAmountPlayScene = (int)(m_iCrossHairMoveYAmountPlayScene / 1.5);
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

	if (Input::IsKeyPressed(VK_R))
	{
		if (m_fPlayerCharacter.m_iAmmo <= m_fPlayerCharacter.m_iMaxAmmo)
			m_fPlayerCharacter.m_iAmmo = m_fPlayerCharacter.m_iMaxAmmo;
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
			if (m_fPlayerCharacter.m_iAmmo <= 0) return;
			m_fPlayerCharacter.m_iAmmo--;

			bCrossHairMove = true;
			m_fPlayerCharacter.m_eAnimationState = Object::EAnimationState::AIMFIRE;

			//Damage
			if (m_fCrossHair.m_fAxis.X > m_fEnemyCharacter.m_fAxis.X && m_fCrossHair.m_fAxis.Y > m_fEnemyCharacter.m_fAxis.Y &&
				m_fCrossHair.m_fAxis.X < m_fEnemyCharacter.m_fAxis.X + strlen(m_fEnemyCharacter.m_fanimation[m_fEnemyCharacter.m_eAnimationState].m_fui[m_fEnemyCharacter.m_iPlaybackCurrentSeconds].m_ppcontent[0]) &&
				m_fCrossHair.m_fAxis.Y < m_fEnemyCharacter.m_fAxis.Y + m_fEnemyCharacter.m_fanimation[m_fEnemyCharacter.m_eAnimationState].m_fui[m_fEnemyCharacter.m_iPlaybackCurrentSeconds].m_ippcontentSize)
			{ 
  				m_fEnemyCharacter.m_iHealth -= m_fPlayerCharacter.m_iFireDamage;
			}
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
		if (m_fPlayerCharacter.m_iAmmo <= 0) return;
		m_fPlayerCharacter.m_iAmmo--;

		bCrossHairMove = true;
		m_fPlayerCharacter.m_eAnimationState = Object::EAnimationState::AIMFIRE;
		
		// Damage
		if (m_fCrossHair.m_fAxis.X > m_fEnemyCharacter.m_fAxis.X && m_fCrossHair.m_fAxis.Y > m_fEnemyCharacter.m_fAxis.Y &&
			m_fCrossHair.m_fAxis.X < m_fEnemyCharacter.m_fAxis.X + strlen(m_fEnemyCharacter.m_fanimation[m_fEnemyCharacter.m_eAnimationState].m_fui[m_fEnemyCharacter.m_iPlaybackCurrentSeconds].m_ppcontent[0]) &&
			m_fCrossHair.m_fAxis.Y < m_fEnemyCharacter.m_fAxis.Y + m_fEnemyCharacter.m_fanimation[m_fEnemyCharacter.m_eAnimationState].m_fui[m_fEnemyCharacter.m_iPlaybackCurrentSeconds].m_ippcontentSize)
		{
			m_fEnemyCharacter.m_iHealth -= m_fPlayerCharacter.m_iFireDamage;
		}

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
		break;
	case SHOOTING:
		//ConsoleRenderer::ScreenDrawUIFromFile(&m_fVideoPlayScene[m_ivideoPlaycursor], FG_WHITE);
		//ConsoleRenderer::ScreenDrawUIFromFile(&PlayerCharacter.m_fanimation[PlayerCharacter.m_eAnimationState].m_fui[PlayerCharacter.m_iPlaybackCurrentSeconds], FG_WHITE);
		//ConsoleRenderer::ScreenDrawUIFromFile(&PlayerCharacter.m_fanimation[1].m_fui[0], FG_WHITE);
		ConsoleRenderer::ScreenDrawPlayerWithAnimation(m_fEnemyCharacter.m_fAxis.X, m_fEnemyCharacter.m_fAxis.Y, &m_fEnemyCharacter.m_fanimation[m_fEnemyCharacter.m_eAnimationState].m_fui[m_fEnemyCharacter.m_iPlaybackCurrentSeconds], m_fEnemyCharacter.m_iColor);
		ConsoleRenderer::ScreenDrawPlayerHealthUI(m_fEnemyCharacter.m_fAxis.X, m_fEnemyCharacter.m_fAxis.Y, &m_fEnemyCharacter.m_fHealthBar, m_fEnemyCharacter.m_iHealth, m_fEnemyCharacter.m_iMaxHealth,  m_fEnemyCharacter.m_fHealthBar.m_iUIColor);
		ConsoleRenderer::ScreenDrawPlayerWithAnimation(m_fPlayerCharacter.m_fAxis.X, m_fPlayerCharacter.m_fAxis.Y, &m_fPlayerCharacter.m_fanimation[m_fPlayerCharacter.m_eAnimationState].m_fui[m_fPlayerCharacter.m_iPlaybackCurrentSeconds], m_fPlayerCharacter.m_iColor);
		ConsoleRenderer::ScreenDrawPlayerHealthUI(m_fPlayerCharacter.m_fAxis.X, m_fPlayerCharacter.m_fAxis.Y, &m_fPlayerCharacter.m_fHealthBar, m_fPlayerCharacter.m_iHealth, m_fPlayerCharacter.m_iMaxHealth, m_fPlayerCharacter.m_fHealthBar.m_iUIColor);
		break;
	case RHYTHM:
		break;
	case PUZZLE:
		break;
	default:
		break;
	}

	ConsoleRenderer::ScreenDrawPlayerLeftAmmo(m_fLeftAmmoUIPlayScene.m_fAxis.X, m_fLeftAmmoUIPlayScene.m_fAxis.Y, &m_fLeftAmmoUIPlayScene.m_fui, m_fPlayerCharacter.m_iAmmo, m_fPlayerCharacter.m_iMaxAmmo, m_fLeftAmmoUIPlayScene.m_iColor);
	ConsoleRenderer::ScreenDrawUIFromFile(&m_fGuideUIPlayScene, m_fGuideUIPlayScene.m_iUIColor);
	Object::RenderCrossHair(&m_fCrossHair);
	Object::RenderAllNode(m_pBulletHead, m_fLeftAmmoUIPlayScene.m_iColor);
}
