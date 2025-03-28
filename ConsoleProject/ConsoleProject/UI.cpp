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