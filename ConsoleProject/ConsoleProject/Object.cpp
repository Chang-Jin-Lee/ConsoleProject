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

	Node* Add(Node* Root, FActor data, COORD dirVector, COORD DestinationVector, float speed) // data를 가지는 Node를 생성해서 붙이기
	{
		Node* pAlloc = (Node*)malloc(sizeof(Node));
		pAlloc->data = data;
		pAlloc->next = Root; //0
		pAlloc->DirVector = dirVector;
		pAlloc->DestinationVector = DestinationVector;
		pAlloc->Speed = speed;
		Root = pAlloc;

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
			cur->data.m_fAxis.X += cur->DirVector.X * deltatime;
			cur->data.m_fAxis.Y += cur->DirVector.Y * deltatime;
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
		int width = ConsoleRenderer::ScreenWidth() * pCross->m_fAspectRatio;
		int height = width * 2;

		pCross->Top.m_fui.m_ippcontentSize = height;
		pCross->Bottom.m_fui.m_ippcontentSize = height;
		pCross->Right.m_fui.m_ippcontentSize = width;
		pCross->Left.m_fui.m_ippcontentSize = width;

		pCross->Top.m_fAxis.X = pCross->m_fAxis.X;
		pCross->Top.m_fAxis.Y = pCross->m_fAxis.Y - height;

		pCross->Bottom.m_fAxis.X = pCross->m_fAxis.X;
		pCross->Bottom.m_fAxis.Y = pCross->m_fAxis.Y + height;

		pCross->Right.m_fAxis.X = pCross->m_fAxis.X + height;
		pCross->Right.m_fAxis.Y = pCross->m_fAxis.Y;

		pCross->Left.m_fAxis.X = pCross->m_fAxis.X - height;
		pCross->Left.m_fAxis.Y = pCross->m_fAxis.Y;

		pCross->Top.m_fui.m_ppcontent = (char**)malloc(sizeof(char*) * height);
		for (int i = 0; i < height; i++)
		{
			pCross->Top.m_fui.m_ppcontent[i] = (char*)malloc(sizeof(char) * width);
			for (int j = 0; j < width; j++)
			{
				pCross->Top.m_fui.m_ppcontent[i][j] = 'A';
			}
			pCross->Top.m_fui.m_ppcontent[i][width - 1] = '\0';
		}

		pCross->Bottom.m_fui.m_ppcontent = (char**)malloc(sizeof(char*) * height);
		for (int i = 0; i < height; i++)
		{
			pCross->Bottom.m_fui.m_ppcontent[i] = (char*)malloc(sizeof(char) * width);
			for (int j = 0; j < width; j++)
			{
				pCross->Bottom.m_fui.m_ppcontent[i][j] = 'A';
			}
			pCross->Bottom.m_fui.m_ppcontent[i][width - 1] = '\0';
		}

		pCross->Left.m_fui.m_ppcontent = (char**)malloc(sizeof(char*) * width);
		for (int i = 0; i < width; i++)
		{
			pCross->Left.m_fui.m_ppcontent[i] = (char*)malloc(sizeof(char) * height);
			for (int j = 0; j < height; j++)
			{
				pCross->Left.m_fui.m_ppcontent[i][j] = 'A';
			}
			pCross->Left.m_fui.m_ppcontent[i][height - 1] = '\0';
		}

		pCross->Right.m_fui.m_ppcontent = (char**)malloc(sizeof(char*) * width);
		for (int i = 0; i < width; i++)
		{
			pCross->Right.m_fui.m_ppcontent[i] = (char*)malloc(sizeof(char) * height);
			for (int j = 0; j < height; j++)
			{
				pCross->Right.m_fui.m_ppcontent[i][j] = 'A';
			}
			pCross->Right.m_fui.m_ppcontent[i][height - 1] = '\0';
		}
	}

	void UpdateCrossHair(FCrossHair* pCross)
	{

	}

	void RenderCrossHair(FCrossHair* pCross)
	{
		pCross->Top.m_fAxis.X = pCross->m_fAxis.X - strlen(pCross->Top.m_fui.m_ppcontent[0]) / 2;
		pCross->Top.m_fAxis.Y = pCross->m_fAxis.Y - pCross->Top.m_fui.m_ippcontentSize * 2;

		pCross->Bottom.m_fAxis.X = pCross->m_fAxis.X - strlen(pCross->Bottom.m_fui.m_ppcontent[0]) / 2;
		pCross->Bottom.m_fAxis.Y = pCross->m_fAxis.Y + pCross->Top.m_fui.m_ippcontentSize * 2;

		pCross->Right.m_fAxis.X = pCross->m_fAxis.X + pCross->Top.m_fui.m_ippcontentSize * 2;
		pCross->Right.m_fAxis.Y = pCross->m_fAxis.Y;

		pCross->Left.m_fAxis.X = pCross->m_fAxis.X - pCross->Top.m_fui.m_ippcontentSize * 2;
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