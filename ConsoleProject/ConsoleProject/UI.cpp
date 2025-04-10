#include "UI.h"
#include "FileController.h"

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
				for (int j = 0; j < video->m_fui[j].m_ippcontentSize; j++)
					free(video->m_fui[i].m_ppcontent[j]);
				if(video->m_fui[i].m_ppcontent)
					free(video->m_fui[i].m_ppcontent);
			}
		}
		
	}

	void CreateBubbleUI(FUI* ui, int width, int height, int xAxis, int yAxis, float thickNess, int color)
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
				ui->m_ppcontent[i][j] = (char)L'⬛';
			}
		}
		for (int i = (int)(SpeechSlateYSize * (1 - BubbleTicknessRatio)); i < SpeechSlateYSize; i++)
		{
			for (int j = 0; j < SpeechSlateXSize; j++)
			{
				ui->m_ppcontent[i][j] = (char)L'⬛';
			}
		}
		for (int j = 0; j < SpeechSlateXSize * BubbleTicknessRatio / 2; j++)
		{
			for (int i = int(SpeechSlateYSize * BubbleTicknessRatio); i < int(SpeechSlateYSize * (1 - BubbleTicknessRatio)); i++)
			{
				ui->m_ppcontent[i][j] = (char)L'⬛';
			}
		}
		for (int j = (int)(SpeechSlateXSize * (1 - BubbleTicknessRatio / 2)); j < SpeechSlateXSize; j++)
		{
			for (int i = int(SpeechSlateYSize * BubbleTicknessRatio); i < int(SpeechSlateYSize * (1 - BubbleTicknessRatio)); i++)
			{
				ui->m_ppcontent[i][j] = (char)L'⬛';
			}
		}
		for (int i = 0; i < SpeechSlateYSize; i++)
		{
			ui->m_ppcontent[i][SpeechSlateXSize + 1] = '\0';
		}
	}

	void CreateBubbleUIContent(UI::FBUBBLEUI* ui, const char* filename,  int color)
	{
		if (FileController::FileRead(filename, "r", &ui->m_fcontent.m_ppcontent, &ui->m_fcontent.m_ippcontentSize))
		{
			SHORT x = ui->m_fbackGround.m_fAxis.X;
			SHORT y = ui->m_fbackGround.m_fAxis.Y;
			size_t width = strlen(ui->m_fbackGround.m_ppcontent[0]);
			size_t height = ui->m_fbackGround.m_ippcontentSize;
			ui->m_fcontent.m_iUIColor = color;

			ui->m_fcontent.m_fAxis.X = x + SHORT(width * 0.5) - SHORT(strlen(ui->m_fcontent.m_ppcontent[0]) * 0.5);
			ui->m_fcontent.m_fAxis.Y = y + SHORT(height * 0.3);
		}
	}

	void CreateBulletUI(UI::FUI* ui, int width, int height, int color)
	{
		int SpeechSlateYSize = height;
		int SpeechSlateXSize = width;
		ui->m_ppcontent = (char**)malloc(sizeof(char*) * (SpeechSlateYSize + 2));
		for (int i = 0; i < SpeechSlateYSize; i++)
		{
			ui->m_ppcontent[i] = (char*)malloc(sizeof(char) * (SpeechSlateXSize + 2));
			if(ui->m_ppcontent)
				memset(ui->m_ppcontent[i], ' ', SpeechSlateXSize);
		}
		ui->m_ippcontentSize = SpeechSlateYSize;
		ui->m_iUIColor = color;

		// 실제 내용 채우기 (둥글게)
		for (int i = 0; i < SpeechSlateYSize; i++)
		{
			for (int j = 0; j < SpeechSlateXSize; j++)
			{
				// 가장자리에서 떨어진 정도
				bool isCorner =
					(i == 0 && (j < 2 || j >= SpeechSlateXSize - 2)) || // 윗 모서리
					(i == SpeechSlateYSize - 1 && (j < 2 || j >= SpeechSlateXSize - 2)) || // 아랫 모서리
					(j == 0 && (i < 2 || i >= SpeechSlateYSize - 2)) || // 왼쪽 모서리
					(j == SpeechSlateXSize - 1 && (i < 2 || i >= SpeechSlateYSize - 2)); // 오른쪽 모서리

				if (!isCorner)
				{
					ui->m_ppcontent[i][j] = (char)L'⬛'; // 꽉 채우기
				}
			}
		}
	}

	void CreateStoneUI(UI::FUI* ui, int width, int height, int color)
	{
		int SpeechSlateYSize = height;
		int SpeechSlateXSize = width;
		ui->m_ppcontent = (char**)malloc(sizeof(char*) * (SpeechSlateYSize + 1));

		for (int i = 0; i < SpeechSlateYSize; i++)
		{
			if (ui->m_ppcontent)
			{
				ui->m_ppcontent[i] = (char*)malloc(sizeof(char) * (SpeechSlateXSize + 1));
				memset(ui->m_ppcontent[i], ' ', SpeechSlateXSize); // 기본값: 공백
				ui->m_ppcontent[i][SpeechSlateXSize] = '\0'; // 널 종료
			}
		}
		ui->m_ippcontentSize = SpeechSlateYSize;
		ui->m_iUIColor = color;

		const char symbols[] =
			"@!#%$^&*()_+=-~`<>?/\\|{}[]:;\"',.1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz█▓▒■◆●◼◾▪★☆※♠♣♥♦∞ΩΣ∆¶£¢¥¤⊙◎◉↺⇔⌛⌚☠☢☣⛔";
		// 총 문자 수
		const int numSymbols = sizeof(symbols) - 1; // '\0' 제외

		// 실제 내용 채우기 (둥글게)
		for (int i = 0; i < SpeechSlateYSize; i++)
		{
			for (int j = 0; j < SpeechSlateXSize; j++)
			{
				// 가장자리에서 떨어진 정도
				bool isCorner =
					(i == 0 && (j < 2 || j >= SpeechSlateXSize - 2)) || // 윗 모서리
					(i == SpeechSlateYSize - 1 && (j < 2 || j >= SpeechSlateXSize - 2)) || // 아랫 모서리
					(j == 0 && (i < 2 || i >= SpeechSlateYSize - 2)) || // 왼쪽 모서리
					(j == SpeechSlateXSize - 1 && (i < 2 || i >= SpeechSlateYSize - 2)); // 오른쪽 모서리

				if (!isCorner)
				{
					ui->m_ppcontent[i][j] = symbols[rand() % numSymbols];
				}
				else
				{
					ui->m_ppcontent[i][j] = ' '; // 모서리는 공백 처리
				}
			}
		}
	}

	FCOORDSNode* AddCOORDNode(FCOORDSNode* Root, COORD data) // data를 가지는 Node를 생성해서 붙이기
	{
		FCOORDSNode* pAlloc = (FCOORDSNode*)malloc(sizeof(FCOORDSNode));
		if (pAlloc == NULL) return Root;
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