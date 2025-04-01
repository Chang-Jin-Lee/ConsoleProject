#include "FileController.h"
#include <stdio.h>
#include <corecrt_malloc.h>
#include <string.h>
#include "ConsoleRenderer.h"

char g_cszBuff[MAX_WORD_LENGTH];	// 입력 버퍼
wchar_t g_cszWBuff[MAX_WORD_LENGTH];	// 입력 버퍼

namespace FileController
{
	int FileRead(const char* FileName, const char* Mode, char*** outBuffer, size_t* outBuffer_size)
	{
		FILE* fp = NULL;
		fopen_s(&fp, FileName, Mode);
		if (fp == NULL)
		{
			ConsoleRenderer::print((char*)"File Dose not Opened.\n");
			return 0;
		}

		*outBuffer = (char**)malloc(sizeof(char*) * (MAX_BUFFER_POOL_SIZE + 1));

		size_t m_ioutBufferIndex = 0;
		
		while (true)
		{
			memset(g_cszBuff, '\0', MAX_WORD_LENGTH);
			fgets(g_cszBuff, MAX_WORD_LENGTH, fp);
			if (feof(fp)) break;

			size_t g_cszBuff_size = strlen(g_cszBuff);
			(*outBuffer)[m_ioutBufferIndex] = (char*)malloc(sizeof(char) * (g_cszBuff_size + 1));

			if((*outBuffer)[m_ioutBufferIndex])
				memcpy_s((*outBuffer)[m_ioutBufferIndex], g_cszBuff_size, g_cszBuff, g_cszBuff_size);
			
			if((*outBuffer)[m_ioutBufferIndex])
				(*outBuffer)[m_ioutBufferIndex][g_cszBuff_size] = NULL;

			m_ioutBufferIndex++;
		}

		*outBuffer_size = m_ioutBufferIndex;

		fclose(fp);

		return 1;
	}

	int FileRead(const char* FileName, const char* Mode, wchar_t*** outBuffer, size_t* outBuffer_size)
	{
		FILE* fp = NULL;
		fopen_s(&fp, FileName, Mode);
		if (fp == NULL)
		{
			ConsoleRenderer::print((char*)"File Dose not Opened.\n");
			return 0;
		}

		*outBuffer = (wchar_t**)malloc(sizeof(wchar_t*) * (MAX_BUFFER_POOL_SIZE + 1));

		size_t m_ioutBufferIndex = 0;

		while (true)
		{
			memset(g_cszWBuff, '\0', MAX_WORD_LENGTH);
			fgetws(g_cszWBuff, MAX_WORD_LENGTH, fp);
			if (feof(fp)) break;

			size_t g_cszBuff_size = wcslen(g_cszWBuff);
			(*outBuffer)[m_ioutBufferIndex] = (wchar_t*)malloc(sizeof(wchar_t) * (g_cszBuff_size + 1));

			if ((*outBuffer)[m_ioutBufferIndex])
				memcpy_s((*outBuffer)[m_ioutBufferIndex], g_cszBuff_size, g_cszWBuff, g_cszBuff_size);

			if((*outBuffer)[m_ioutBufferIndex])
				(*outBuffer)[m_ioutBufferIndex][g_cszBuff_size] = NULL;

			m_ioutBufferIndex++;
		}

		*outBuffer_size = m_ioutBufferIndex;

		fclose(fp);

		return 1;
	}

	int FileReadFromCSV(const char* FileName, const char* Mode, UI::FGAMEDIALOG* dialog, int* dialogSize)
	{
		char filename[200];
		sprintf_s(filename, sizeof(filename), "CSV/%s", FileName);

		FILE* fp = NULL;
		fopen_s(&fp, filename, Mode);

		if (fp == NULL)
		{
			ConsoleRenderer::print((char*)"FileReadFromCSV: File Dose not Opened.\n");
			return 0;
		}

		int m_ioutBufferIndex = 0;

		while (true)
		{
			memset(g_cszBuff, '\0', MAX_WORD_LENGTH);
			fgets(g_cszBuff, MAX_WORD_LENGTH, fp);
			if (feof(fp)) break;

			size_t g_cszBuff_size = strlen(g_cszBuff);
			char* context = NULL;
			char* ptr = strtok_s(g_cszBuff, ",", &context);
			char** ptrs = (char**)malloc(sizeof(char*) * MAX_PTR_SIZE);
			int count = 0;

			while (ptr != NULL)
			{
				size_t ptrSize = strlen(ptr) + 1;
				ptrs[count] = (char*)malloc(sizeof(char) * ptrSize);
				memcpy_s(ptrs[count++], ptrSize, ptr, ptrSize);
				ConsoleRenderer::print(ptr);
				ptr = strtok_s(NULL, ",", &context);
			}

			if (ptrs)
			{
				dialog[m_ioutBufferIndex].InSize = atoi(ptrs[0]);
				dialog[m_ioutBufferIndex].Speaker.m_pcontent = ptrs[1];
				dialog[m_ioutBufferIndex].Speaker.m_ipcontentSize = strlen(ptrs[1]);
				dialog[m_ioutBufferIndex].Dialog.m_pcontent = ptrs[2];
				dialog[m_ioutBufferIndex].Dialog.m_ipcontentSize = strlen(ptrs[2]);
				dialog[m_ioutBufferIndex].Type.m_pcontent = ptrs[3];
				dialog[m_ioutBufferIndex].Type.m_ipcontentSize = strlen(ptrs[3]);
				dialog[m_ioutBufferIndex].Answer.m_pcontent = ptrs[4];
				dialog[m_ioutBufferIndex].Answer.m_ipcontentSize = strlen(ptrs[4]);
				dialog[m_ioutBufferIndex].Likeability.m_pcontent = ptrs[5];
				dialog[m_ioutBufferIndex].Likeability.m_ipcontentSize = strlen(ptrs[5]);
				ptrs[6][strlen(ptrs[6]) - 1] = '\0';
				dialog[m_ioutBufferIndex].SceneName.m_pcontent = ptrs[6];
				dialog[m_ioutBufferIndex].SceneName.m_ipcontentSize = strlen(ptrs[6]) - 1;
			}

			m_ioutBufferIndex++;
		}

		*dialogSize = m_ioutBufferIndex;

		fclose(fp);

		return 1;
	}

	int FileReadFromCSV_Dialogue(const char* FileName, const char* Mode, UI::FUI* speechBubble, UI::FGAMEDIALOGANIMATIONSCENE* dialog, int* dialogSize)
	{
		char filename[200];
		sprintf_s(filename, sizeof(filename), "CSV/%s.csv", FileName);

		FILE* fp = NULL;
		fopen_s(&fp, filename, Mode);

		if (fp == NULL)
		{
			ConsoleRenderer::print((char*)"FileReadFromCSV: File Dose not Opened.\n");
			return 0;
		}

		int outBufferIndex = 0;

		while (true)
		{
			memset(g_cszBuff, '\0', MAX_WORD_LENGTH);
			fgets(g_cszBuff, MAX_WORD_LENGTH, fp);
			if (feof(fp)) break;

			size_t g_cszBuff_size = strlen(g_cszBuff);
			char* context = NULL;
			char* ptr = strtok_s(g_cszBuff, ",", &context);
			char** ptrs = (char**)malloc(sizeof(char*) * MAX_PTR_SIZE);
			int count = 0;

			while (ptr != NULL)
			{
				size_t ptrSize = strlen(ptr) + 1;
				ptrs[count] = (char*)malloc(sizeof(char) * ptrSize);
				memcpy_s(ptrs[count++], ptrSize, ptr, ptrSize);
				ConsoleRenderer::print(ptr);
				ptr = strtok_s(NULL, ",", &context);
			}

			if (ptrs)
			{
				dialog[outBufferIndex].Number = atoi(ptrs[0]);
				
				dialog[outBufferIndex].m_iTalkingCharacterSize = atoi(ptrs[1]);
				// Speaker
				char* tContext = NULL;
				char* tempPtrs = strtok_s(ptrs[2], ";", &tContext);
				while (tempPtrs != NULL)
				{
					if (!strcmp(tempPtrs, "rapi"))
						dialog[outBufferIndex].m_aSpeakerTalkable[ECharacterName::Rapi] = true;
					if (!strcmp(tempPtrs, "anis"))
						dialog[outBufferIndex].m_aSpeakerTalkable[ECharacterName::Anis] = true;
					if (!strcmp(tempPtrs , "neon"))
						dialog[outBufferIndex].m_aSpeakerTalkable[ECharacterName::Neon] = true;
					if (!strcmp(tempPtrs , "shifty"))
						dialog[outBufferIndex].m_aSpeakerTalkable[ECharacterName::Shifty] = true;
					tempPtrs = strtok_s(NULL, ";", &tContext);
				}

				tContext = NULL;
				tempPtrs = strtok_s(ptrs[3], ";", &tContext);
				int idx = 0;
				while (tempPtrs != NULL)
				{
					size_t tempPtrSize = strlen(tempPtrs) + 1;
					dialog[outBufferIndex].m_sSelectDialogue[idx] = (char*)malloc(sizeof(char) * tempPtrSize);
					memcpy_s(dialog[outBufferIndex].m_sSelectDialogue[idx], tempPtrSize, tempPtrs, tempPtrSize);
					tempPtrs = strtok_s(NULL, ";", &tContext);
					idx++;
				}
				dialog[outBufferIndex].NextIdx = atoi(ptrs[4]);

				dialog[outBufferIndex].SelectNextDialogue[0] = atoi(ptrs[5]);
				dialog[outBufferIndex].SelectNextDialogue[1] = atoi(ptrs[6]);

				dialog[outBufferIndex].m_iChoiceSize = atoi(ptrs[8]);
			}

			outBufferIndex++;
		}

		*dialogSize = outBufferIndex;
		fclose(fp);

		for (int i = 0; i < outBufferIndex; i++)
		{
			// Speaker 초기화
			dialog[i].m_fspeaker.m_fAxis.X = speechBubble->m_fAxis.X + strlen(speechBubble->m_ppcontent[0]) * 0.1;
			dialog[i].m_fspeaker.m_fAxis.Y = speechBubble->m_fAxis.Y + speechBubble->m_ippcontentSize * 0.15;
			dialog[i].m_fspeaker.m_iUIColor = FG_WHITE;

			char filename[50];
			if (dialog[i+1].m_aSpeakerTalkable[ECharacterName::Rapi] == true)
				sprintf_s(filename, sizeof(filename), "Images/text/%s/Speaker/text_rapi_30.txt", FileName);
			if (dialog[i+1].m_aSpeakerTalkable[ECharacterName::Anis] == true)
				sprintf_s(filename, sizeof(filename), "Images/text/%s/Speaker/text_anis_30.txt", FileName);	
			if (dialog[i+1].m_aSpeakerTalkable[ECharacterName::Neon] == true)
				sprintf_s(filename, sizeof(filename), "Images/text/%s/Speaker/text_neon_30.txt", FileName);
			if (dialog[i+1].m_aSpeakerTalkable[ECharacterName::Shifty] == true)
				sprintf_s(filename, sizeof(filename), "Images/text/%s/Speaker/text_shifty_30.txt", FileName);
			//sprintf_s(filename, sizeof(filename), "Images/text/text_description%04d_360.txt", i + 1);
			if (FileRead(filename, "r", &dialog[i + 1].m_fspeaker.m_ppcontent, &dialog[i + 1].m_fspeaker.m_ippcontentSize) == false)
			{
				ConsoleRenderer::print((char*)"FileRead m_fspeaker Error : ");
				ConsoleRenderer::print(filename);
				ConsoleRenderer::print((char*)"\n");
			}

			// Dialogue 초기화
			dialog[i].m_fDialogue.m_fAxis.X = speechBubble->m_fAxis.X + strlen(speechBubble->m_ppcontent[0]) * 0.15;
			dialog[i].m_fDialogue.m_fAxis.Y = speechBubble->m_fAxis.Y + speechBubble->m_ippcontentSize * 0.4;
			dialog[i].m_fDialogue.m_iUIColor = FG_WHITE;

			sprintf_s(filename, sizeof(filename), "Images/text/%s/text_%04d_12.txt", FileName, i + 1);	// font 사이즈를 변경하려면 여기 파일 이름을 변경하기
			//sprintf_s(filename, sizeof(filename), "Images/text/text_description%04d_360.txt", i + 1);
			if (FileRead(filename, "r", &dialog[i+1].m_fDialogue.m_ppcontent, &dialog[i+1].m_fDialogue.m_ippcontentSize) == false)
			{
				ConsoleRenderer::print((char*)"FileRead Error : ");
				ConsoleRenderer::print(filename);
				ConsoleRenderer::print((char*)"\n");
			}

			// Initialize SelectBubble
			for (int j = 0; j < MAX_SELECTBUBBLE_SIZE; j++)
			{
				if (dialog[i].m_sSelectDialogue[j] != NULL)
				{
					UI::CreateBubbleUI(&dialog[i].m_fSelectBubble[j].m_fbackGround,
						(int)(ConsoleRenderer::ScreenWidth() * 0.3),
						(int)(ConsoleRenderer::ScreenHeight() * 0.05),
						(int)(ConsoleRenderer::ScreenWidth() * 0.65),
						(int)(ConsoleRenderer::ScreenHeight() * (0.55 + 0.06 * float(j))),
						0.01f,
						FG_RED
					);

					sprintf_s(filename, sizeof(filename), "Images/text/%s.txt", dialog[i].m_sSelectDialogue[j]);
					UI::CreateBubbleUIContent(&dialog[i].m_fSelectBubble[j], filename, FG_WHITE);
				}
			}
		}

		return 1;
	}

	int FileReadVideo(const char* VideoName, UI::FVideo* video)
	{
		for (int i = 0; i < MAX_VIDEO_SIZE; i++)
		{
			char filename[200];
			sprintf_s(filename, sizeof(filename), "Video/%s/frame_%04d.txt", VideoName, i + 1);

			FILE* fp = NULL;
			fopen_s(&fp, filename, "r");
			if (fp == NULL)
			{
				video->m_iMaxLength = i;
				ConsoleRenderer::print(filename);
				ConsoleRenderer::print((char*)"File Dose not Opened.\n");
				break;
			}

			video->m_fui[i].m_ppcontent = (char**)malloc(sizeof(char*) * (MAX_BUFFER_POOL_SIZE + 1));

			int m_ioutBufferIndex = 0;

			while (true)
			{
				memset(g_cszBuff, '\0', MAX_WORD_LENGTH);
				fgets(g_cszBuff, MAX_WORD_LENGTH, fp);
				if (feof(fp)) break;

				size_t g_cszBuff_size = strlen(g_cszBuff);
				video->m_fui[i].m_ppcontent[m_ioutBufferIndex] = (char*)malloc(sizeof(char) * (g_cszBuff_size + 1));

				for (int i = 0; i < g_cszBuff_size; i++)
					if(video->m_fui[i].m_ppcontent[m_ioutBufferIndex])	
						video->m_fui[i].m_ppcontent[m_ioutBufferIndex][i] = g_cszBuff[i];
				if(video->m_fui[i].m_ppcontent[m_ioutBufferIndex])
					if(video->m_fui[i].m_ppcontent[m_ioutBufferIndex])
						video->m_fui[i].m_ppcontent[m_ioutBufferIndex][g_cszBuff_size] = NULL;

				m_ioutBufferIndex++;
			}

			video->m_fui[i].m_ippcontentSize = m_ioutBufferIndex;
			fclose(fp);
		}
		return 1;
	}

	int FileReadAnimation(const char* VideoName, const int& animationState, Object::FPlayerCharacter* pc)
	{
		int dFileRead = true;
		for (int i = 0; i < MAX_VIDEO_SIZE; i++)
		{
			char filename[100];
			sprintf_s(filename, sizeof(filename), "Video/%s/frame_%04d.txt", VideoName, i + 1);

			FILE* fp = NULL;
			fopen_s(&fp, filename, "r");
			if (fp == NULL)
			{
				pc->m_fanimation[animationState].m_iMaxLength = i;
				ConsoleRenderer::print(filename);
				ConsoleRenderer::print((char*)"File Dose not Opened.\n");
				dFileRead = false;
				break;
			}
			
			pc->m_fanimation[animationState].m_fui[i].m_ppcontent = (char**)malloc(sizeof(char*) * (MAX_BUFFER_POOL_SIZE + 1));
			int m_ioutBufferIndex = 0;

			while (true)
			{
				memset(g_cszBuff, '\0', MAX_WORD_LENGTH);
				fgets(g_cszBuff, MAX_WORD_LENGTH, fp);
				if (feof(fp)) break;

				size_t g_cszBuff_size = strlen(g_cszBuff);
				pc->m_fanimation[animationState].m_fui[i].m_ppcontent[m_ioutBufferIndex] = (char*)malloc(sizeof(char) * (g_cszBuff_size + 1));

				for (int j = 0; j < g_cszBuff_size; j++)
				{
					if(pc->m_fanimation[animationState].m_fui[i].m_ppcontent[m_ioutBufferIndex])
						pc->m_fanimation[animationState].m_fui[i].m_ppcontent[m_ioutBufferIndex][j] = g_cszBuff[j];
				}
				if(pc->m_fanimation[animationState].m_fui[i].m_ppcontent[m_ioutBufferIndex])
					pc->m_fanimation[animationState].m_fui[i].m_ppcontent[m_ioutBufferIndex][g_cszBuff_size] = NULL;

				m_ioutBufferIndex++;
			}

			pc->m_fanimation[animationState].m_fui[i].m_ippcontentSize = m_ioutBufferIndex;
			fclose(fp);
		}

		return dFileRead == true;
	}
}

