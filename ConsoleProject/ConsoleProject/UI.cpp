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
}