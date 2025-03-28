#pragma once

#include "Windows.h"
#include "UI.h"

namespace Object
{
#define MAX_ANIMATIONSTATE_SIZE 5
	enum EAnimationState
	{
		FULLBODY_IDLE = 0,
		COVER,
		AIM,
		AIMFIRE,
		RELOAD
	};

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
	};

	typedef struct FPlayerCharacter	// PlayerCharacter에 대한 것. 애니메이션 포함
	{
		COORD m_fAxis;

		EAnimationState m_eAnimationState = AIM;
		Animation m_fanimation[MAX_ANIMATIONSTATE_SIZE];

		bool m_bPlayable;
		int m_iPlaybackCurrentSeconds;
		int m_iColor;

		FPlayerCharacter()
		{
			m_fAxis.X = 0;
			m_fAxis.Y = 0;
			m_bPlayable = false;
			m_iPlaybackCurrentSeconds = 0;
		}

		FPlayerCharacter(int x, int y)
		{
			m_fAxis.X = x;
			m_fAxis.Y = y;
			m_bPlayable = false;
			m_iPlaybackCurrentSeconds = 0;
		}

		FPlayerCharacter(int x, int y, char* content)
		{
			m_fAxis.X = x;
			m_fAxis.Y = y;
			m_bPlayable = false;
			m_iPlaybackCurrentSeconds = 0;
		}
	};

	typedef struct FActor	// Actor에 대한 것. 총알 등
	{
		COORD m_fAxis;
		UI::FUI m_fui;
		int m_iColor;

		FActor()
		{
			m_fAxis.X = 0;
			m_fAxis.Y = 0;
		}

		FActor(int x, int y)
		{
			m_fAxis.X = x;
			m_fAxis.Y = y;
		}

		FActor(int x, int y, char* content)
		{
			m_fAxis.X = x;
			m_fAxis.Y = y;
		}

		FActor(int x, int y, const FActor& actor)
		{
			m_fAxis.X = x;
			m_fAxis.Y = y;
			m_fui = actor.m_fui;
		}

		FActor(const FActor& actor)
		{
			m_fAxis.X = actor.m_fAxis.X;
			m_fAxis.Y = actor.m_fAxis.Y;
			m_fui = actor.m_fui;
		}
	};

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
	void LoadAnimationData(FPlayerCharacter* pc);
	void Release(FPlayerCharacter* pc);
}