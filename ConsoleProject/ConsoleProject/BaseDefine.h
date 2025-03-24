#pragma once
#include "Time.h"

typedef enum class ESceneState {
	MENU,
	PLAY,
	END,
	MAX
};

namespace UI
{
	typedef struct FUI	// 이중포인터쓰는 UI ex) 파일에서 읽은 스트링, 대화창 틀 등
	{
		COORD m_fAxis;
		char* m_pcontent = NULL;
		char** m_ppcontent = NULL;
		int m_icontentSize = -1;

		FUI() {}

		FUI(int x, int y)
		{
			m_fAxis.X = x;
			m_fAxis.Y = y;
		}

		FUI(int x, int y, char* content)
		{
			m_fAxis.X = x;
			m_fAxis.Y = y;
			m_pcontent = content;
		}

	};
}

#define VK_Q 0x41 + 'q' - 'a'
#define VK_W 0x41 + 'w' - 'a'
#define VK_E 0x41 + 'e' - 'a'