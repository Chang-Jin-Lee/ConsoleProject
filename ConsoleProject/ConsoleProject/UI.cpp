#include "UI.h"

namespace UI
{
	void Release(FUI* ui)
	{
		if (ui->m_pcontent)
			free(ui->m_pcontent);

		if (ui->m_ppcontent)
		{
			for (int i = 0; i < ui->m_ippcontentSize; i++)
				free(ui->m_ppcontent[i]);
			free(ui->m_ppcontent);
		}
	}

	void Release(FVideo* video)
	{
		for (int i = 0; i < video->m_iMaxLength; i++)
		{
			if (video->m_fui[i].m_ppcontent)
			{
				for (int i = 0; i < video->m_fui[i].m_ippcontentSize; i++)
					free(video->m_fui[i].m_ppcontent[i]);
				free(video->m_fui[i].m_ppcontent);
			}
		}
		
	}

	void MakeBubbleUI(FUI* ui, int width, int height, int xAxis, int yAxis, float thickNess, int color)
	{
		float BubbleTicknessRatio = thickNess;
		int SpeechSlateYSize = height;
		int SpeechSlateXSize = width;
		ui->m_ppcontent = (char**)malloc(sizeof(char*) * (SpeechSlateYSize + 2));
		for (int i = 0; i < SpeechSlateYSize; i++)
		{
			ui->m_ppcontent[i] = (char*)malloc(sizeof(char) * (SpeechSlateXSize + 2));
			memset(ui->m_ppcontent[i], ' ', SpeechSlateXSize);
		}
		ui->m_ippcontentSize = SpeechSlateYSize;
		ui->m_fAxis.X = xAxis;
		ui->m_fAxis.Y = yAxis;
		ui->m_iUIColor = color;

		for (int i = 0; i < SpeechSlateYSize * BubbleTicknessRatio; i++)
		{
			for (int j = 0; j < SpeechSlateXSize; j++)
			{
				ui->m_ppcontent[i][j] = L'⬛';
			}
		}
		for (int i = SpeechSlateYSize * (1 - BubbleTicknessRatio); i < SpeechSlateYSize; i++)
		{
			for (int j = 0; j < SpeechSlateXSize; j++)
			{
				ui->m_ppcontent[i][j] = L'⬛';
			}
		}
		for (int j = 0; j < SpeechSlateXSize * BubbleTicknessRatio / 2; j++)
		{
			for (int i = SpeechSlateYSize * BubbleTicknessRatio; i < SpeechSlateYSize * (1 - BubbleTicknessRatio); i++)
			{
				ui->m_ppcontent[i][j] = L'⬛';
			}
		}
		for (int j = SpeechSlateXSize * (1 - BubbleTicknessRatio / 2); j < SpeechSlateXSize; j++)
		{
			for (int i = SpeechSlateYSize * BubbleTicknessRatio; i < SpeechSlateYSize * (1 - BubbleTicknessRatio); i++)
			{
				ui->m_ppcontent[i][j] = L'⬛';
			}
		}
		for (int i = 0; i < SpeechSlateYSize; i++)
		{
			ui->m_ppcontent[i][SpeechSlateXSize + 1] = '\0';
		}
	}

	FCOORDSNode* AddCOORDNode(FCOORDSNode* Root, COORD data) // data를 가지는 Node를 생성해서 붙이기
	{
		FCOORDSNode* pAlloc = (FCOORDSNode*)malloc(sizeof(FCOORDSNode));
		pAlloc->data = data;
		pAlloc->next = Root; //0
		Root = pAlloc;

		return Root;
	}

	void DeleteCOORDNode(FCOORDSNode* curNode, COORD data)
	{
		FCOORDSNode* Target = curNode;
		curNode = curNode->next;

		free(Target);
	}

	void TravelNodeCOORDNode(FCOORDSNode* Root)
	{
		if (Root == NULL)
			return;

		FCOORDSNode* cur = Root;

		while (cur != NULL)
			cur = cur->next;
	}
}