#pragma once

#include "Windows.h"
#include "UI.h"
#include "ConsoleRenderer.h"

namespace Object
{
#define MAX_ANIMATIONSTATE_SIZE 7
	typedef enum EAnimationState
	{
		COVER = 0,
		AIM,
		AIMFIRE,
		RELOAD,
		FULLBODYIDLE,
		FULLBODYTALK,
		FULLBODYEXPRESSION,
		
	} EAnimationState;

	typedef struct Animation
	{
		UI::FUI m_fui[MAX_VIDEO_SIZE];
		char* m_pAnimationName;
		int m_iMaxLength;

		Animation()
		{
			for (int i = 0; i < MAX_ANIMATIONSTATE_SIZE; i++)
			{
				m_pAnimationName = NULL;
				m_iMaxLength = 0;
			}
		}
	} Animation;

	typedef struct FPlayerCharacter	// PlayerCharacter에 대한 것. 애니메이션 포함
	{
		#pragma region Property
		COORD m_fAxis;
		UI::FUI m_fHealthBar;
		EAnimationState m_eAnimationState = AIM;
		Animation m_fanimation[MAX_ANIMATIONSTATE_SIZE];

		bool m_bVisible;
		bool m_bPlayable;
		bool m_bCanFire;
		int m_iPlaybackCurrentSeconds;
		int m_iColor;
		int m_iHealth;
		int m_iMaxHealth;
		int m_iAmmo;		// 현재 탄약집에 있는 총알 수
		int m_iMaxAmmo;		// 장전을 위한 탄약집의 총알 수
		int m_iFireDamage; // 총알 데미지
		int m_iFireDelayTime;
		#pragma endregion

		FPlayerCharacter()
		{
			m_fAxis.X = 0;
			m_fAxis.Y = 0;
			m_bVisible = false;
			m_bPlayable = false;
			m_bCanFire = false;
			m_iPlaybackCurrentSeconds = 0;
			m_iColor = FG_WHITE;
			m_iHealth = 0;
			m_iMaxHealth = 0;
			m_iAmmo = 0;
			m_iMaxAmmo = 0;
			m_iFireDamage = 0;
			m_iFireDelayTime = 0;
		}

		FPlayerCharacter(int x, int y)
		{
			m_fAxis.X = x;
			m_fAxis.Y = y;
			m_bVisible = false;
			m_bPlayable = false;
			m_bCanFire = false;
			m_iPlaybackCurrentSeconds = 0;
			m_iColor = FG_WHITE;
			m_iHealth = 0;
			m_iMaxHealth = 0;
			m_iAmmo = 0;
			m_iMaxAmmo = 0;
			m_iFireDamage = 0;
			m_iFireDelayTime = 0;
		}

		FPlayerCharacter(int x, int y, char* content)
		{
			m_fAxis.X = x;
			m_fAxis.Y = y;
			m_bVisible = false;
			m_bPlayable = false;
			m_bCanFire = false;
			m_iPlaybackCurrentSeconds = 0;
			m_iColor = FG_WHITE;
			m_iHealth = 0;
			m_iMaxHealth = 0;
			m_iAmmo = 0;
			m_iMaxAmmo = 0;
			m_iFireDamage = 0;
			m_iFireDelayTime = 0;
		}
	} FPlayerCharacter;

	typedef struct FActor	// Actor에 대한 것. 총알 등
	{
		COORD m_fAxis;
		UI::FUI m_fui;
		float m_fSpawnTime;
		FPlayerCharacter* m_pOwnerCharacter;
		int m_iColor;

		FActor()
		{
			m_fAxis.X = 0;
			m_fAxis.Y = 0;
			m_iColor = FG_WHITE;
			m_pOwnerCharacter = NULL;
		}

		FActor(int x, int y)
		{
			m_fAxis.X = x;
			m_fAxis.Y = y;
			m_iColor = FG_WHITE;
			m_pOwnerCharacter = NULL;
		}

		FActor(int x, int y, char* content)
		{
			m_fAxis.X = x;
			m_fAxis.Y = y;
			m_iColor = FG_WHITE;
			m_pOwnerCharacter = NULL;
		}

		FActor(int x, int y, const FActor& actor)
		{
			m_fAxis.X = x;
			m_fAxis.Y = y;
			m_fui = actor.m_fui;
			m_iColor = FG_WHITE;
			m_pOwnerCharacter = NULL;
		}

		FActor(const FActor& actor)
		{
			m_fAxis.X = actor.m_fAxis.X;
			m_fAxis.Y = actor.m_fAxis.Y;
			m_fui = actor.m_fui;
			m_iColor = FG_WHITE;
			m_pOwnerCharacter = NULL;
		}
	} FActor;

	typedef struct FCrossHair
	{
		COORD m_fAxis;
		SHORT m_sXDistanceFromCenter;
		SHORT m_sYDistanceFromCenter;
		float m_fAspectRatio;
		int m_iColor;
		bool bEnable;
		FActor Left;
		FActor Top;
		FActor Right;
		FActor Bottom;

		FCrossHair()
		{
			m_fAxis = {-1,-1};
			m_fAspectRatio = 0.01f;
			bEnable = false;
			m_iColor = FG_WHITE;
			m_sXDistanceFromCenter = 0;
			m_sYDistanceFromCenter = 0;
		}

	}FCrossHair;

	typedef struct FLeftAmmoUI	// LeftAmmoUI에 대한 것. 총알 등
	{
		COORD m_fAxis;
		UI::FUI m_fui;
		int m_iColor;

		FLeftAmmoUI()
		{
			m_fAxis.X = 0;
			m_fAxis.Y = 0;
			m_iColor = FG_WHITE;
		}

		FLeftAmmoUI(int x, int y)
		{
			m_fAxis.X = x;
			m_fAxis.Y = y;
			m_iColor = FG_WHITE;
		}

		FLeftAmmoUI(int x, int y, char* content)
		{
			m_fAxis.X = x;
			m_fAxis.Y = y;
			m_iColor = FG_WHITE;
		}

		FLeftAmmoUI(int x, int y, const FLeftAmmoUI& leftammoui)
		{
			m_fAxis.X = x;
			m_fAxis.Y = y;
			m_fui = leftammoui.m_fui;
			m_iColor = FG_WHITE;
		}

		FLeftAmmoUI(const FLeftAmmoUI& leftammoui)
		{
			m_fAxis.X = leftammoui.m_fAxis.X;
			m_fAxis.Y = leftammoui.m_fAxis.Y;
			m_fui = leftammoui.m_fui;
			m_iColor = FG_WHITE;
		}
	} FLeftAmmoUI;

	typedef struct BulletNode
	{
		FActor data;
		COORD DirVector;
		COORD DestinationVector;
		float Speed;
		struct BulletNode* next;
	} BulletNode;

	typedef struct StoneNode
	{
		FActor data;
		COORD DirVector;
		COORD DestinationVector;
		float Speed;
		struct StoneNode* next;
	} StoneNode;

	BulletNode* AddBulletNode(BulletNode* Root, FActor* data, COORD dirVector, COORD DestinationVector, float speed, int color); // data를 가지는 Node를 생성해서 붙이기
	BulletNode* DeleteBulletNode(BulletNode* curNode);
	BulletNode* DeleteAllNodeBulletNode(BulletNode* Root);
	void TravelNodeBulletNode(BulletNode* Root);
	void RenderAllBulletNodeBulletNode(BulletNode* Root);
	BulletNode* UpdateAllNodeAxisBulletNode(BulletNode* Root, float deltatime);

	StoneNode* AddStoneNode(StoneNode* Root, FActor* data, COORD dirVector, COORD DestinationVector, float speed, int color); // data를 가지는 Node를 생성해서 붙이기
	StoneNode* DeleteStoneNode(StoneNode* curNode);
	StoneNode* DeleteAllNodeStoneNode(StoneNode* Root);
	void TravelNodeStoneNode(StoneNode* Root);
	void RenderAllStoneNodeStoneNode(StoneNode* Root);
	StoneNode* UpdateAllNodeAxisStoneNode(StoneNode* Root, FPlayerCharacter* ch, FPlayerCharacter* enemy, float deltatime);

	
	void SetPlayerAnimationName(FPlayerCharacter* pc, char* fullbody_dile, char* cover = NULL, char* aim = NULL, char* aimfire = NULL, char* reload = NULL);	// 각 폴더의 이름을 설정해주자
	void SetPlayerAnimationNameFullBody(FPlayerCharacter* pc, char* fullbody_dile, char* fullbody_talk = NULL, char* fullbody_expression = NULL);
	void CreateAndAttachHealthBar(FPlayerCharacter* pc, COORD axis, int Color);
	void LoadAnimationData(FPlayerCharacter* pc);
	void LoadAnimationDataWithFrame(FPlayerCharacter* pc, float frameTickness);
	void LoadBulletData(UI::FUI* ui);


	void Release(FPlayerCharacter* pc);
	void Release(FActor* actor);

	void CreateCrossHair(FCrossHair* pCross);
	void UpdateCrossHairIncreaseSize(FCrossHair* pCross, float deltatime);
	void UpdateCrossHairDecreaseSize(FCrossHair* pCross, float deltatime);
	void RenderCrossHair(FCrossHair* pCross);
}