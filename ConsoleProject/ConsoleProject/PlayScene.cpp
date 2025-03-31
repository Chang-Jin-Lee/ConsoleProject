#include "PlayScene.h"
#include "Input.h"
#include "Game.h"
#include "UI.h"
#include "Object.h"

// Timer 관련
float m_fcurrentTimePlayScene = 0;
float m_fAnimationTimePlayScene = 1/60;
float m_fMenuLastTimePlayScene = 0;
float m_fcurrentFireDelayTimePlayScene = 0;


enum EPlaySceneState
{
	DESCRIPTION,
	OPTION,
	SHOOTING,
	RHYTHM,
	PUZZLE,
};
enum ECharacterName
{
	Rapi, Anis, Neon
};

EPlaySceneState m_eplaySceneState = DESCRIPTION;

/////// DESCRIPTION ///////
#define MAX_DESCRIPTION_SIZE_PLAY_SCENE 3
UI::FBUBBLEUI m_fDescriptionUIPlayScene[MAX_DESCRIPTION_SIZE_PLAY_SCENE];	// height를 18로 맞추자.

/////// SHOOTING ///////
#define MAX_CHARACTER_SIZE 3
bool bIsReloading = false;
Object::FPlayerCharacter m_fPlayerCharacter[MAX_CHARACTER_SIZE];
int m_icharacterIndex = 0;
Object::FPlayerCharacter m_fEnemyCharacter;
Object::FLeftAmmoUI m_fLeftAmmoUIPlayScene;
Object::FCrossHair m_fCrossHair;
Object::Node* m_pBulletHead = NULL;
float m_fDefaultBulletSpeed = 10.0f;
UI::FUI m_fGuideUIPlayScene; // 설명 UI

Object::FActor m_fBullet;

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

void PlayScene::Initialize()	// 게임 시작할 때 초기화
{
	DescriptionPlaySceneInitialize();
	ShootingPlaySceneInitialize();
	OptionPlaySceneInitialize();
}

void PlayScene::LoadData()	// 각 애니메이션에 대한 데이터를 읽어온다
{
	DescriptionPlaySceneLoadData();
	ShootingPlaySceneLoadData();
	OptionPlaySceneLoadData();
}

void PlayScene::ProcessInput()
{
	switch (m_eplaySceneState)
	{
	case DESCRIPTION:
		DescriptionPlaySceneInput();
		break;
	case OPTION:
		OptionPlaySceneInput();
		break;
	case SHOOTING:
		ShootingPlaySceneInput();
		break;
	case RHYTHM:
		break;
	case PUZZLE:
		break;
	default:
		break;
	}
}

void PlayScene::Release()
{
	for(int i = 0 ; i < MAX_CHARACTER_SIZE; i++)
		Object::Release(&m_fPlayerCharacter[i]);
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

	switch (m_eplaySceneState)
	{
	case DESCRIPTION:
		DescriptionPlaySceneUpdate();
		break;
	case OPTION:
		OptionPlaySceneUpdate();
		break;
	case SHOOTING:
		ShootingPlaySceneUpdate();
		break;
	case RHYTHM:
		break;
	case PUZZLE:
		break;
	default:
		break;
	}
}

void PlayScene::Render()
{
	switch (m_eplaySceneState)
	{
	case DESCRIPTION:
		DescriptionPlaySceneRender();
		break;
	case OPTION:
		OptionPlaySceneRender();
		break;
	case SHOOTING:
		ShootingPlaySceneRender();
		break;
	case RHYTHM:
		break;
	case PUZZLE:
		break;
	default:
		break;
	}
}

void PlayScene::DescriptionPlaySceneInitialize()
{
	for (int i = 0; i < MAX_DESCRIPTION_SIZE_PLAY_SCENE; i++)
	{
		// Initialize Guide UI
		UI::CreateBubbleUI(&m_fDescriptionUIPlayScene[i].m_fbackGround,
			(int)(ConsoleRenderer::ScreenWidth() * 0.6),
			(int)(ConsoleRenderer::ScreenHeight() * 0.1),
			(int)(ConsoleRenderer::ScreenWidth() * 0.2),
			(int)(ConsoleRenderer::ScreenHeight() * (0.15 + 0.125 * float(i))),
			0.02f,
			FG_WHITE
		);

		char filename[100];
		sprintf_s(filename, sizeof(filename), "Images/text/text_description%04d_360.txt", i + 1);
		UI::CreateBubbleUIContent(&m_fDescriptionUIPlayScene[i], filename, FG_WHITE);
	}
}

void PlayScene::DescriptionPlaySceneLoadData()
{
}

void PlayScene::DescriptionPlaySceneUpdate()
{
}

void PlayScene::DescriptionPlaySceneInput()
{
	if (Input::IsKeyPressed(VK_Q))
	{
		m_eplaySceneState = SHOOTING;
	}
}

void PlayScene::DescriptionPlaySceneRender()
{
	for (int i = 0; i < MAX_DESCRIPTION_SIZE_PLAY_SCENE; i++)
	{
		ConsoleRenderer::ScreenDrawUIFromFile(&m_fDescriptionUIPlayScene[i].m_fbackGround, m_fDescriptionUIPlayScene[i].m_fbackGround.m_iUIColor);
		ConsoleRenderer::ScreenDrawUIFromFile(&m_fDescriptionUIPlayScene[i].m_fcontent, m_fDescriptionUIPlayScene[i].m_fcontent.m_iUIColor);
	}
}

void PlayScene::OptionPlaySceneInitialize()
{
}

void PlayScene::OptionPlaySceneLoadData()
{
}

void PlayScene::OptionPlaySceneUpdate()
{
}

void PlayScene::OptionPlaySceneInput()
{
	if (Input::IsKeyPressed(VK_Q))
	{
		m_eplaySceneState = SHOOTING;
	}
}

void PlayScene::OptionPlaySceneRender()
{
}

void PlayScene::ShootingPlaySceneInitialize()
{
	// Initialize Player - rapi
	m_fPlayerCharacter[ECharacterName::Rapi].m_eAnimationState = Object::EAnimationState::COVER;
	m_fPlayerCharacter[ECharacterName::Rapi].m_bPlayable = true;
	m_fPlayerCharacter[ECharacterName::Rapi].m_fAxis.X = ConsoleRenderer::ScreenCenter(m_fPlayerCharacter[ECharacterName::Rapi].m_fanimation[m_fPlayerCharacter[ECharacterName::Rapi].m_eAnimationState].m_fui->m_ppcontent[0]);
	m_fPlayerCharacter[ECharacterName::Rapi].m_fAxis.Y = (SHORT)(ConsoleRenderer::ScreenHeight() * 0.52);
	Object::CreateAndAttachHealthBar(&m_fPlayerCharacter[ECharacterName::Rapi], COORD{ 0,SHORT(ConsoleRenderer::ScreenHeight() * 0.005) }, FG_GREEN);
	m_fPlayerCharacter[ECharacterName::Rapi].m_iColor = FG_WHITE;
	m_fPlayerCharacter[ECharacterName::Rapi].m_iHealth = 100;
	m_fPlayerCharacter[ECharacterName::Rapi].m_iMaxHealth = 100;
	m_fPlayerCharacter[ECharacterName::Rapi].m_iAmmo = 30;
	m_fPlayerCharacter[ECharacterName::Rapi].m_iMaxAmmo = 30;
	m_fPlayerCharacter[ECharacterName::Rapi].m_iFireDamage = 2;
	m_fPlayerCharacter[ECharacterName::Rapi].m_iFireDelayTime = 0.08;

	// Initialize Player - anis
	m_fPlayerCharacter[ECharacterName::Anis].m_eAnimationState = Object::EAnimationState::COVER;
	m_fPlayerCharacter[ECharacterName::Anis].m_bPlayable = true;
	m_fPlayerCharacter[ECharacterName::Anis].m_fAxis.X = ConsoleRenderer::ScreenCenter(m_fPlayerCharacter[ECharacterName::Anis].m_fanimation[m_fPlayerCharacter[ECharacterName::Anis].m_eAnimationState].m_fui->m_ppcontent[0]);
	m_fPlayerCharacter[ECharacterName::Anis].m_fAxis.Y = (SHORT)(ConsoleRenderer::ScreenHeight() * 0.52);
	Object::CreateAndAttachHealthBar(&m_fPlayerCharacter[ECharacterName::Anis], COORD{ 0,SHORT(ConsoleRenderer::ScreenHeight() * 0.005) }, FG_GREEN);
	m_fPlayerCharacter[ECharacterName::Anis].m_iColor = FG_WHITE;
	m_fPlayerCharacter[ECharacterName::Anis].m_iHealth = 100;
	m_fPlayerCharacter[ECharacterName::Anis].m_iMaxHealth = 100;
	m_fPlayerCharacter[ECharacterName::Anis].m_iAmmo = 10;
	m_fPlayerCharacter[ECharacterName::Anis].m_iMaxAmmo = 10;
	m_fPlayerCharacter[ECharacterName::Anis].m_iFireDamage = 12;
	m_fPlayerCharacter[ECharacterName::Anis].m_iFireDelayTime = 1;

	// Initialize Player - neon
	m_fPlayerCharacter[ECharacterName::Neon].m_eAnimationState = Object::EAnimationState::COVER;
	m_fPlayerCharacter[ECharacterName::Neon].m_bPlayable = true;
	m_fPlayerCharacter[ECharacterName::Neon].m_fAxis.X = ConsoleRenderer::ScreenCenter(m_fPlayerCharacter[ECharacterName::Neon].m_fanimation[m_fPlayerCharacter[ECharacterName::Neon].m_eAnimationState].m_fui->m_ppcontent[0]);
	m_fPlayerCharacter[ECharacterName::Neon].m_fAxis.Y = (SHORT)(ConsoleRenderer::ScreenHeight() * 0.52);
	Object::CreateAndAttachHealthBar(&m_fPlayerCharacter[ECharacterName::Neon], COORD{ 0,SHORT(ConsoleRenderer::ScreenHeight() * 0.005) }, FG_GREEN);
	m_fPlayerCharacter[ECharacterName::Neon].m_iColor = FG_WHITE;
	m_fPlayerCharacter[ECharacterName::Neon].m_iHealth = 100;
	m_fPlayerCharacter[ECharacterName::Neon].m_iMaxHealth = 100;
	m_fPlayerCharacter[ECharacterName::Neon].m_iAmmo = 150;
	m_fPlayerCharacter[ECharacterName::Neon].m_iMaxAmmo = 150;
	m_fPlayerCharacter[ECharacterName::Neon].m_iFireDamage = 1;
	m_fPlayerCharacter[ECharacterName::Neon].m_iFireDelayTime = 0.03;

	// Initialize Enemy 
	m_fEnemyCharacter.m_eAnimationState = Object::EAnimationState::FULLBODYIDLE;
	m_fEnemyCharacter.m_bPlayable = true;
	m_fEnemyCharacter.m_fAxis.X = ConsoleRenderer::ScreenCenter(m_fEnemyCharacter.m_fanimation[m_fEnemyCharacter.m_eAnimationState].m_fui->m_ppcontent[0]);
	m_fEnemyCharacter.m_fAxis.Y = (SHORT)(-ConsoleRenderer::ScreenHeight() * 0.1);
	Object::CreateAndAttachHealthBar(&m_fEnemyCharacter, COORD{ 0,SHORT(ConsoleRenderer::ScreenHeight() * 0.12) }, FG_RED);
	m_fEnemyCharacter.m_iColor = FG_WHITE;
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
	m_fLeftAmmoUIPlayScene.m_fAxis.X = (SHORT)(m_fPlayerCharacter[m_icharacterIndex].m_fAxis.X - ConsoleRenderer::ScreenWidth() * 0.05);
	m_fLeftAmmoUIPlayScene.m_fAxis.Y = (SHORT)(m_fPlayerCharacter[m_icharacterIndex].m_fAxis.Y + ConsoleRenderer::ScreenHeight() * 0.12);
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

void PlayScene::ShootingPlaySceneLoadData()
{
	Object::SetPlayerAnimationName(&m_fPlayerCharacter[0], (char*)"RapiFullBody", (char*)"RapiCover", (char*)"RapiAim", (char*)"RapiAimFire", (char*)"RapiReload");
	Object::LoadAnimationData(&m_fPlayerCharacter[0]);
	Object::SetPlayerAnimationName(&m_fPlayerCharacter[1], (char*)"AnisFullBody", (char*)"AnisCover", (char*)"AnisAim", (char*)"AnisAimFire", (char*)"AnisReload");
	Object::LoadAnimationData(&m_fPlayerCharacter[1]);
	Object::SetPlayerAnimationName(&m_fPlayerCharacter[2], (char*)"NeonFullBody", (char*)"NeonCover", (char*)"NeonAim", (char*)"NeonAimFire", (char*)"NeonReload");
	Object::LoadAnimationData(&m_fPlayerCharacter[2]);

	Object::SetPlayerAnimationName(&m_fEnemyCharacter, (char*)"ModerniaFullBody");
	Object::LoadAnimationData(&m_fEnemyCharacter);

	// Ammoui
	int LeftAmmoYSize = (int)(ConsoleRenderer::ScreenHeight() * 0.08);
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
		m_fLeftAmmoUIPlayScene.m_fui.m_ppcontent[i][LeftAmmoXSize - 1] = '\0';
	}
}

void PlayScene::ShootingPlaySceneUpdate()
{
	if (m_fEnemyCharacter.m_iHealth <= 0)
	{
		Game::ChangeScene(ESceneState::END);
	}
	m_fMenuLastTimePlayScene = Time::GetTotalTime() - m_fcurrentTimePlayScene;
	if (m_fMenuLastTimePlayScene >= m_fAnimationTimePlayScene)
	{
		m_fcurrentTimePlayScene = Time::GetTotalTime();

		if (m_fPlayerCharacter[m_icharacterIndex].m_bPlayable)
		{
			if (bIsReloading && m_fPlayerCharacter[m_icharacterIndex].m_eAnimationState == Object::EAnimationState::RELOAD)
				if (m_fPlayerCharacter[m_icharacterIndex].m_iPlaybackCurrentSeconds >= m_fPlayerCharacter[m_icharacterIndex].m_fanimation[m_fPlayerCharacter[m_icharacterIndex].m_eAnimationState].m_iMaxLength - 1)
					bIsReloading = false;
			m_fPlayerCharacter[m_icharacterIndex].m_iPlaybackCurrentSeconds = (m_fPlayerCharacter[m_icharacterIndex].m_iPlaybackCurrentSeconds + 1) % m_fPlayerCharacter[m_icharacterIndex].m_fanimation[m_fPlayerCharacter[m_icharacterIndex].m_eAnimationState].m_iMaxLength;
		}

		if (m_fEnemyCharacter.m_bPlayable)
			m_fEnemyCharacter.m_iPlaybackCurrentSeconds = (m_fEnemyCharacter.m_iPlaybackCurrentSeconds + 1) % m_fEnemyCharacter.m_fanimation[m_fEnemyCharacter.m_eAnimationState].m_iMaxLength;
	}

	if (Time::GetTotalTime() - m_fcurrentFireDelayTimePlayScene >= m_fPlayerCharacter[m_icharacterIndex].m_iFireDelayTime)
	{
		m_fcurrentFireDelayTimePlayScene = Time::GetTotalTime();
		m_fPlayerCharacter[m_icharacterIndex].m_bCanFire = true;
	}

	Object::UpdateAllNodeAxis(m_pBulletHead, Time::GetElapsedTime());
	if (bCrossHairMove)
	{
		if (m_fCrossHair.m_sXDistanceFromCenter < m_icrossHairTargetXSize)
			Object::UpdateCrossHairIncreaseSize(&m_fCrossHair, Time::GetElapsedTime());
		else
			Object::UpdateCrossHairDecreaseSize(&m_fCrossHair, Time::GetElapsedTime());
	}
}


void PlayScene::ShootingPlaySceneInput()
{
	if (Input::IsKeyPressed(VK_1))
	{
		m_icharacterIndex = 0;
	}
	if (Input::IsKeyPressed(VK_2))
	{
		m_icharacterIndex = 1;
	}
	if (Input::IsKeyPressed(VK_3))
	{
		m_icharacterIndex = 2;
	}
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
		if (m_fPlayerCharacter[m_icharacterIndex].m_iAmmo <= m_fPlayerCharacter[m_icharacterIndex].m_iMaxAmmo)
			m_fPlayerCharacter[m_icharacterIndex].m_iAmmo = m_fPlayerCharacter[m_icharacterIndex].m_iMaxAmmo;
		m_fPlayerCharacter[m_icharacterIndex].m_eAnimationState = Object::EAnimationState::RELOAD;
		bIsReloading = true;
	}

	if (bIsReloading) return;

	if (Input::IsKeyPressed(VK_E))
	{
		m_fPlayerCharacter[m_icharacterIndex].m_bPlayable = !m_fPlayerCharacter[m_icharacterIndex].m_bPlayable;
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
			if (m_fPlayerCharacter[m_icharacterIndex].m_iAmmo <= 0) return;
			if (!m_fPlayerCharacter[m_icharacterIndex].m_bCanFire) return;
			m_fPlayerCharacter[m_icharacterIndex].m_bCanFire = false;
			m_fPlayerCharacter[m_icharacterIndex].m_iAmmo--;
			bCrossHairMove = true;
			m_fPlayerCharacter[m_icharacterIndex].m_eAnimationState = Object::EAnimationState::AIMFIRE;

			//Damage
			if (m_fCrossHair.m_fAxis.X > m_fEnemyCharacter.m_fAxis.X && m_fCrossHair.m_fAxis.Y > m_fEnemyCharacter.m_fAxis.Y &&
				m_fCrossHair.m_fAxis.X < m_fEnemyCharacter.m_fAxis.X + strlen(m_fEnemyCharacter.m_fanimation[m_fEnemyCharacter.m_eAnimationState].m_fui[m_fEnemyCharacter.m_iPlaybackCurrentSeconds].m_ppcontent[0]) &&
				m_fCrossHair.m_fAxis.Y < m_fEnemyCharacter.m_fAxis.Y + m_fEnemyCharacter.m_fanimation[m_fEnemyCharacter.m_eAnimationState].m_fui[m_fEnemyCharacter.m_iPlaybackCurrentSeconds].m_ippcontentSize)
			{
				m_fEnemyCharacter.m_iHealth -= m_fPlayerCharacter[m_icharacterIndex].m_iFireDamage;
			}
		}
		else
		{
			bCrossHairMove = false;
			m_fPlayerCharacter[m_icharacterIndex].m_eAnimationState = Object::EAnimationState::AIM;
		}
	}

	if (!Input::IsKeyDown(VK_RIGHT) && !Input::IsKeyDown(VK_LEFT) && !Input::IsKeyDown(VK_UP) && !Input::IsKeyDown(VK_DOWN) && !Input::IsKeyDown(VK_SPACE))
	{
		m_fPlayerCharacter[m_icharacterIndex].m_eAnimationState = Object::EAnimationState::COVER;
	}

	if (Input::IsKeyPressed(VK_Z))
	{
		m_fPlayerCharacter[m_icharacterIndex].m_eAnimationState = Object::EAnimationState::COVER;
	}
	if (Input::IsKeyPressed(VK_X))
	{
		m_fPlayerCharacter[m_icharacterIndex].m_eAnimationState = Object::EAnimationState::AIM;
	}
	if (Input::IsKeyPressed(VK_C))
	{
		m_fPlayerCharacter[m_icharacterIndex].m_eAnimationState = Object::EAnimationState::COVER;
	}

	if (Input::IsKeyDown(VK_SPACE))
	{
		if (m_fPlayerCharacter[m_icharacterIndex].m_iAmmo <= 0) return;
		if (!m_fPlayerCharacter[m_icharacterIndex].m_bCanFire) return;
		m_fPlayerCharacter[m_icharacterIndex].m_bCanFire = false;
		m_fPlayerCharacter[m_icharacterIndex].m_iAmmo--;

		bCrossHairMove = true;
		m_fPlayerCharacter[m_icharacterIndex].m_eAnimationState = Object::EAnimationState::AIMFIRE;

		// Damage
		if (m_fCrossHair.m_fAxis.X > m_fEnemyCharacter.m_fAxis.X && m_fCrossHair.m_fAxis.Y > m_fEnemyCharacter.m_fAxis.Y &&
			m_fCrossHair.m_fAxis.X < m_fEnemyCharacter.m_fAxis.X + strlen(m_fEnemyCharacter.m_fanimation[m_fEnemyCharacter.m_eAnimationState].m_fui[m_fEnemyCharacter.m_iPlaybackCurrentSeconds].m_ppcontent[0]) &&
			m_fCrossHair.m_fAxis.Y < m_fEnemyCharacter.m_fAxis.Y + m_fEnemyCharacter.m_fanimation[m_fEnemyCharacter.m_eAnimationState].m_fui[m_fEnemyCharacter.m_iPlaybackCurrentSeconds].m_ippcontentSize)
		{
			m_fEnemyCharacter.m_iHealth -= m_fPlayerCharacter[m_icharacterIndex].m_iFireDamage;
		}

		// Effect Spawn

		// Bullet Spawn
		//Object::FActor Actor = Object::FActor(m_fCrossHair.m_fAxis.X + strlen(m_fCrossHair.m_fui.m_ppcontent[0]) / 2, m_fCrossHair.m_fAxis.Y + m_fCrossHair.m_fui.m_ippcontentSize / 2, m_fBullet);
		//Object::FActor Actor = Object::FActor( ConsoleRenderer::ScreenWidth() / 2, m_fPlayerCharacter[m_icharacterIndex].m_fAxis.Y, m_fBullet);
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

void PlayScene::ShootingPlaySceneRender()
{
	//ConsoleRenderer::ScreenDrawUIFromFile(&m_fVideoPlayScene[m_ivideoPlaycursor], FG_WHITE);
	//ConsoleRenderer::ScreenDrawUIFromFile(&PlayerCharacter.m_fanimation[PlayerCharacter.m_eAnimationState].m_fui[PlayerCharacter.m_iPlaybackCurrentSeconds], FG_WHITE);
	//ConsoleRenderer::ScreenDrawUIFromFile(&PlayerCharacter.m_fanimation[1].m_fui[0], FG_WHITE);
	ConsoleRenderer::ScreenDrawPlayerWithAnimation(m_fEnemyCharacter.m_fAxis.X, m_fEnemyCharacter.m_fAxis.Y, &m_fEnemyCharacter.m_fanimation[m_fEnemyCharacter.m_eAnimationState].m_fui[m_fEnemyCharacter.m_iPlaybackCurrentSeconds], m_fEnemyCharacter.m_iColor);
	ConsoleRenderer::ScreenDrawPlayerHealthUI(m_fEnemyCharacter.m_fAxis.X, m_fEnemyCharacter.m_fAxis.Y, &m_fEnemyCharacter.m_fHealthBar, m_fEnemyCharacter.m_iHealth, m_fEnemyCharacter.m_iMaxHealth, m_fEnemyCharacter.m_fHealthBar.m_iUIColor);
	ConsoleRenderer::ScreenDrawPlayerWithAnimation(m_fPlayerCharacter[m_icharacterIndex].m_fAxis.X, m_fPlayerCharacter[m_icharacterIndex].m_fAxis.Y, &m_fPlayerCharacter[m_icharacterIndex].m_fanimation[m_fPlayerCharacter[m_icharacterIndex].m_eAnimationState].m_fui[m_fPlayerCharacter[m_icharacterIndex].m_iPlaybackCurrentSeconds], m_fPlayerCharacter[m_icharacterIndex].m_iColor);
	ConsoleRenderer::ScreenDrawPlayerHealthUI(m_fPlayerCharacter[m_icharacterIndex].m_fAxis.X, m_fPlayerCharacter[m_icharacterIndex].m_fAxis.Y, &m_fPlayerCharacter[m_icharacterIndex].m_fHealthBar, m_fPlayerCharacter[m_icharacterIndex].m_iHealth, m_fPlayerCharacter[m_icharacterIndex].m_iMaxHealth, m_fPlayerCharacter[m_icharacterIndex].m_fHealthBar.m_iUIColor);
	ConsoleRenderer::ScreenDrawPlayerLeftAmmo(m_fLeftAmmoUIPlayScene.m_fAxis.X, m_fLeftAmmoUIPlayScene.m_fAxis.Y, &m_fLeftAmmoUIPlayScene.m_fui, m_fPlayerCharacter[m_icharacterIndex].m_iAmmo, m_fPlayerCharacter[m_icharacterIndex].m_iMaxAmmo, m_fLeftAmmoUIPlayScene.m_iColor);
	ConsoleRenderer::ScreenDrawUIFromFile(&m_fGuideUIPlayScene, m_fGuideUIPlayScene.m_iUIColor);
	Object::RenderCrossHair(&m_fCrossHair);
	Object::RenderAllBulletNode(m_pBulletHead, m_fLeftAmmoUIPlayScene.m_iColor);
}
