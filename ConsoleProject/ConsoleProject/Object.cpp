#include "Object.h"
#include "FileController.h"
#include "ConsoleRenderer.h"

namespace Object
{
	void SetPlayerAnimationName(FPlayerCharacter* pc, char* fullbody_dile, char* cover, char* aim, char* aimfire, char* reload)
	{
		pc->m_fanimation[EAnimationState::FULLBODY_IDLE].m_pAnimationName = fullbody_dile;
		pc->m_fanimation[EAnimationState::COVER].m_pAnimationName = cover;
		pc->m_fanimation[EAnimationState::AIM].m_pAnimationName = aim;
		pc->m_fanimation[EAnimationState::AIMFIRE].m_pAnimationName = aimfire;
		pc->m_fanimation[EAnimationState::RELOAD].m_pAnimationName = reload;
	}

	void CreateAndAttachHealthBar(FPlayerCharacter* pc, COORD axis, int Color)	// 캐릭터 안에서 상대적 위치
	{
		size_t height = int(ConsoleRenderer::ScreenHeight() * 0.01);
		size_t width = int(ConsoleRenderer::ScreenWidth() * 0.2);
		pc->m_fHealthBar.m_fAxis.Y = pc->m_fAxis.Y + axis.Y;
		pc->m_fHealthBar.m_fAxis.X = ConsoleRenderer::ScreenWidth() / 2 - width / 2 + axis.X;
		pc->m_fHealthBar.m_ippcontentSize = height;
		pc->m_fHealthBar.m_ppcontent = (char**)malloc(sizeof(char*) * height);
		pc->m_fHealthBar.m_iUIColor = Color;
		for (int i = 0; i < height; i++)
		{
			pc->m_fHealthBar.m_ppcontent[i] = (char*)malloc(sizeof(char) * width);
			for (int j = 0; j < width; j++)
			{
				pc->m_fHealthBar.m_ppcontent[i][j] = L'⬛';
			}
			pc->m_fHealthBar.m_ppcontent[i][width - 1] = '\0';
		}
	}

	void LoadAnimationData(FPlayerCharacter* pc)
	{
		for (int i = 0; i < MAX_ANIMATIONSTATE_SIZE; i++)
		{
			if (FileController::FileReadAnimation(pc->m_fanimation[i].m_pAnimationName, i, pc) == false)
			{
				ConsoleRenderer::print((char*)"LoadAnimationData func error...");
			}
		}
	}

	void Release(FPlayerCharacter* pc)
	{
		for (int i = 0; i < MAX_ANIMATIONSTATE_SIZE; i++)
		{
			for (int j = 0; j < pc->m_fanimation[i].m_iMaxLength; j++)
			{
				UI::Release(&pc->m_fanimation[i].m_fui[j]);
			}
		}
	}

	void Release(FActor* actor)
	{
		UI::Release(&actor->m_fui);
	}

	Node* Add(Node* Root, FActor data, COORD dirVector, COORD DestinationVector, float speed) // data를 가지는 Node를 생성해서 붙이기
	{
		Node* pAlloc = (Node*)malloc(sizeof(Node));
		if (pAlloc)
		{
			pAlloc->data = data;
			pAlloc->next = Root; //0
			pAlloc->DirVector = dirVector;
			pAlloc->DestinationVector = DestinationVector;
			pAlloc->Speed = speed;
			Root = pAlloc;
		}

		return Root;
	}

	Node* Delete(Node* curNode)
	{
		Node* Target = curNode;
		curNode = curNode->next;

		free(Target);
		return curNode;
	}

	void TravelNode(Node* Root)
	{
		if (Root == NULL)
			return;

		Node* cur = Root;

		while (cur != NULL)
			cur = cur->next;
	}

	void RenderAllNode(Node* Root, int Color)
	{
		if (Root == NULL)
			return;

		Node* cur = Root;

		while (cur != NULL)
		{
			ConsoleRenderer::ScreenDrawFileStrings(cur->data.m_fAxis.X, cur->data.m_fAxis.Y, cur->data.m_fui.m_ppcontent, cur->data.m_fui.m_ippcontentSize, BG_RED);
			cur = cur->next;
		}
	}

	void UpdateAllNodeAxis(Node* Root, float deltatime)
	{
		if (Root == NULL)
			return;

		Node* cur = Root;

		while (cur != NULL)
		{
			cur->data.m_fAxis.X += (SHORT)(cur->DirVector.X * deltatime);
			cur->data.m_fAxis.Y += (SHORT)(cur->DirVector.Y * deltatime);
			/*if (cur->data.m_fAxis.X < cur->DestinationVector.X || cur->data.m_fAxis.X > ConsoleRenderer::ScreenWidth() || cur->data.m_fAxis.Y < 0 || cur->data.m_fAxis.Y > ConsoleRenderer::ScreenHeight())
			{
				cur = Delete(cur);
				if (cur == NULL) break;
			}*/
			cur = cur->next;
		}
	}


	void CreateCrossHair(FCrossHair* pCross)
	{
		if (pCross == NULL) return;
		int nwidth = (int)(ConsoleRenderer::ScreenWidth() * pCross->m_fAspectRatio);
		int TopBottomWidth = nwidth;
		int TopBottomHeight = nwidth / 2;
		int LeftRightWidth = nwidth;
		int LeftRightHeight = nwidth / 2;
		pCross->m_sXDistanceFromCenter = nwidth / 2;
		pCross->m_sYDistanceFromCenter = nwidth / 4;

		const char crossHairch = L'⬛';

		pCross->Top.m_fui.m_ippcontentSize = TopBottomHeight;
		pCross->Bottom.m_fui.m_ippcontentSize = TopBottomHeight;
		pCross->Right.m_fui.m_ippcontentSize = LeftRightHeight;
		pCross->Left.m_fui.m_ippcontentSize = LeftRightHeight;

		pCross->Top.m_fAxis.X = pCross->m_fAxis.X;
		pCross->Top.m_fAxis.Y = pCross->m_fAxis.Y - TopBottomHeight;

		pCross->Bottom.m_fAxis.X = pCross->m_fAxis.X;
		pCross->Bottom.m_fAxis.Y = pCross->m_fAxis.Y + TopBottomHeight;

		pCross->Right.m_fAxis.X = pCross->m_fAxis.X + LeftRightHeight;
		pCross->Right.m_fAxis.Y = pCross->m_fAxis.Y;

		pCross->Left.m_fAxis.X = pCross->m_fAxis.X - LeftRightHeight;
		pCross->Left.m_fAxis.Y = pCross->m_fAxis.Y;

		pCross->Top.m_fui.m_ppcontent = (char**)malloc(sizeof(char*) * TopBottomHeight);
		if (pCross->Top.m_fui.m_ppcontent)
		{
			for (int i = 0; i < TopBottomHeight; i++)
			{
				pCross->Top.m_fui.m_ppcontent[i] = (char*)malloc(sizeof(char) * TopBottomWidth);
				if (pCross->Top.m_fui.m_ppcontent[i])
				{
					for (int j = 0; j < TopBottomWidth; j++)
					{
						pCross->Top.m_fui.m_ppcontent[i][j] = crossHairch;
					}
				}
				pCross->Top.m_fui.m_ppcontent[i][TopBottomWidth - 1] = '\0';
			}
		}

		pCross->Bottom.m_fui.m_ppcontent = (char**)malloc(sizeof(char*) * TopBottomHeight);
		for (int i = 0; i < TopBottomHeight; i++)
		{
			pCross->Bottom.m_fui.m_ppcontent[i] = (char*)malloc(sizeof(char) * TopBottomWidth);
			if (pCross->Bottom.m_fui.m_ppcontent[i])
			{
				for (int j = 0; j < TopBottomWidth; j++)
				{
					pCross->Bottom.m_fui.m_ppcontent[i][j] = crossHairch;
				}
			}
			pCross->Bottom.m_fui.m_ppcontent[i][TopBottomWidth - 1] = '\0';
		}

		pCross->Left.m_fui.m_ppcontent = (char**)malloc(sizeof(char*) * LeftRightHeight);
		for (int i = 0; i < LeftRightHeight; i++)
		{
			if (pCross->Left.m_fui.m_ppcontent)
			{
				pCross->Left.m_fui.m_ppcontent[i] = (char*)malloc(sizeof(char) * LeftRightWidth);
				for (int j = 0; j < LeftRightWidth; j++)
				{
					if(pCross->Left.m_fui.m_ppcontent[i])
						pCross->Left.m_fui.m_ppcontent[i][j] = crossHairch;
				}
			}
			if (pCross && pCross->Left.m_fui.m_ppcontent)
				pCross->Left.m_fui.m_ppcontent[i][LeftRightWidth - 1] = '\0';
		}

		pCross->Right.m_fui.m_ppcontent = (char**)malloc(sizeof(char*) * LeftRightHeight);
		for (int i = 0; i < LeftRightHeight; i++)
		{
			if (pCross && pCross->Right.m_fui.m_ppcontent)
			{
				pCross->Right.m_fui.m_ppcontent[i] = (char*)malloc(sizeof(char) * LeftRightWidth);
			}
			for (int j = 0; j < LeftRightWidth; j++)
			{
				if (pCross && pCross->Right.m_fui.m_ppcontent[i])
					pCross->Right.m_fui.m_ppcontent[i][j] = crossHairch;
			}
			if (pCross && pCross->Right.m_fui.m_ppcontent[i])
				pCross->Right.m_fui.m_ppcontent[i][LeftRightWidth - 1] = '\0';
		}
	}

	void UpdateCrossHairIncreaseSize(FCrossHair* pCross, float deltatime)
	{
		pCross->m_sXDistanceFromCenter += 2;
		pCross->m_sYDistanceFromCenter += 1;
	}

	void UpdateCrossHairDecreaseSize(FCrossHair* pCross, float deltatime)
	{
		pCross->m_sXDistanceFromCenter -= 2;
		pCross->m_sYDistanceFromCenter -= 1;
	}

	void RenderCrossHair(FCrossHair* pCross)
	{
		if (pCross == NULL) return;
		pCross->Top.m_fAxis.X = pCross->m_fAxis.X;
		pCross->Top.m_fAxis.Y = pCross->m_fAxis.Y - pCross->Top.m_fui.m_ippcontentSize - pCross->m_sYDistanceFromCenter;

		pCross->Bottom.m_fAxis.X = pCross->m_fAxis.X;
		pCross->Bottom.m_fAxis.Y = pCross->m_fAxis.Y + pCross->Top.m_fui.m_ippcontentSize + pCross->m_sYDistanceFromCenter;

		pCross->Right.m_fAxis.X = pCross->m_fAxis.X + pCross->Top.m_fui.m_ippcontentSize + pCross->m_sXDistanceFromCenter;
		pCross->Right.m_fAxis.Y = pCross->m_fAxis.Y;

		pCross->Left.m_fAxis.X = pCross->m_fAxis.X - pCross->Top.m_fui.m_ippcontentSize - pCross->m_sXDistanceFromCenter;
		pCross->Left.m_fAxis.Y = pCross->m_fAxis.Y;

		for (int i = 0; i < pCross->Top.m_fui.m_ippcontentSize; i++)
		{
			ConsoleRenderer::ScreenDrawString(pCross->Top.m_fAxis.X, pCross->Top.m_fAxis.Y + i, pCross->Top.m_fui.m_ppcontent[i], pCross->m_iColor);
		}

		for (int i = 0; i < pCross->Bottom.m_fui.m_ippcontentSize; i++)
		{
			ConsoleRenderer::ScreenDrawString(pCross->Bottom.m_fAxis.X, pCross->Bottom.m_fAxis.Y + i, pCross->Bottom.m_fui.m_ppcontent[i], pCross->m_iColor);
		}
		for (int i = 0; i < pCross->Left.m_fui.m_ippcontentSize; i++)
		{
			ConsoleRenderer::ScreenDrawString(pCross->Left.m_fAxis.X, pCross->Left.m_fAxis.Y + i, pCross->Left.m_fui.m_ppcontent[i], pCross->m_iColor);
		}
		for (int i = 0; i < pCross->Right.m_fui.m_ippcontentSize; i++)
		{
			ConsoleRenderer::ScreenDrawString(pCross->Right.m_fAxis.X, pCross->Right.m_fAxis.Y + i, pCross->Right.m_fui.m_ppcontent[i], pCross->m_iColor);
		}
	}
}