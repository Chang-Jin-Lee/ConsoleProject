#pragma once

#include "Windows.h"
#include "UI.h"
#include "ConsoleRenderer.h"

namespace Object
{
#define MAX_ANIMATIONSTATE_SIZE 5
	typedef enum EAnimationState
	{
		FULLBODY_IDLE = 0,
		COVER,
		AIM,
		AIMFIRE,
		RELOAD
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
		COORD m_fAxis;
		UI::FUI m_fHealthBar;
		EAnimationState m_eAnimationState = AIM;
		Animation m_fanimation[MAX_ANIMATIONSTATE_SIZE];

		bool m_bPlayable;
		int m_iPlaybackCurrentSeconds;
		int m_iColor;
		int m_iHP;
		int m_iAmmo;		// 현재 탄약집에 있는 총알 수
		int m_iMaxAmmo;		// 장전을 위한 탄약집의 총알 수
		int m_iFireDamage; // 총알 데미지

		FPlayerCharacter()
		{
			m_fAxis.X = 0;
			m_fAxis.Y = 0;
			m_bPlayable = false;
			m_iPlaybackCurrentSeconds = 0;
			m_iColor = FG_WHITE;
			m_iHP = 0;
			m_iAmmo = 0;
			m_iMaxAmmo = 0;
			m_iFireDamage = 0;
		}

		FPlayerCharacter(int x, int y)
		{
			m_fAxis.X = x;
			m_fAxis.Y = y;
			m_bPlayable = false;
			m_iPlaybackCurrentSeconds = 0;
			m_iColor = FG_WHITE;
			m_iHP = 0;
			m_iAmmo = 0;
			m_iMaxAmmo = 0;
			m_iFireDamage = 0;

		}

		FPlayerCharacter(int x, int y, char* content)
		{
			m_fAxis.X = x;
			m_fAxis.Y = y;
			m_bPlayable = false;
			m_iPlaybackCurrentSeconds = 0;
			m_iColor = FG_WHITE;
			m_iHP = 0;
			m_iAmmo = 0;
			m_iMaxAmmo = 0;
			m_iFireDamage = 0;

		}
	} FPlayerCharacter;

	typedef struct FActor	// Actor에 대한 것. 총알 등
	{
		COORD m_fAxis;
		UI::FUI m_fui;
		int m_iColor;

		FActor()
		{
			m_fAxis.X = 0;
			m_fAxis.Y = 0;
			m_iColor = FG_WHITE;
		}

		FActor(int x, int y)
		{
			m_fAxis.X = x;
			m_fAxis.Y = y;
			m_iColor = FG_WHITE;
		}

		FActor(int x, int y, char* content)
		{
			m_fAxis.X = x;
			m_fAxis.Y = y;
			m_iColor = FG_WHITE;
		}

		FActor(int x, int y, const FActor& actor)
		{
			m_fAxis.X = x;
			m_fAxis.Y = y;
			m_fui = actor.m_fui;
			m_iColor = FG_WHITE;
		}

		FActor(const FActor& actor)
		{
			m_fAxis.X = actor.m_fAxis.X;
			m_fAxis.Y = actor.m_fAxis.Y;
			m_fui = actor.m_fui;
			m_iColor = FG_WHITE;
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
		}

	}FCrossHair;

	typedef struct Node
	{
		FActor data;
		COORD DirVector;
		COORD DestinationVector;
		float Speed;
		struct Node* next;
	} Node;

	Node* Add(Node* Root, FActor data, COORD dirVector, COORD DestinationVector, float speed); // data를 가지는 Node를 생성해서 붙이기
	Node* Delete(Node* curNode);
	void TravelNode(Node* Root);
	void RenderAllNode(Node* Root, int Color);
	void UpdateAllNodeAxis(Node* Root, float deltatime);
	
	void SetPlayerAnimationName(FPlayerCharacter* pc, char* fullbody_dile, char* cover = NULL, char* aim = NULL, char* aimfire = NULL, char* reload = NULL);	// 각 폴더의 이름을 설정해주자
	void CreateAndAttachHealthBar(FPlayerCharacter* pc, COORD axis, int Color);
	void LoadAnimationData(FPlayerCharacter* pc);
	void Release(FPlayerCharacter* pc);
	void Release(FActor* actor);

	void CreateCrossHair(FCrossHair* pCross);
	void UpdateCrossHairIncreaseSize(FCrossHair* pCross, float deltatime);
	void UpdateCrossHairDecreaseSize(FCrossHair* pCross, float deltatime);
	void RenderCrossHair(FCrossHair* pCross);
}