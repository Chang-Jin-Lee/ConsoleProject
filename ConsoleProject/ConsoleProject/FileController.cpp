#include "FileController.h"
#include <stdio.h>
#include <corecrt_malloc.h>
#include <string.h>
#include "ConsoleRenderer.h"


#define MAX_BUFFER_POOL_SIZE 10001	// 버퍼의 최대 풀 개수 (y 축으로 몇 줄인지.)
#define MAX_WORD_LENGTH 200				// 단어의 최대 알파벳 숫자는 14개, 끝은 문자 NULL (값0)으로 채워야한다.== txt 파일로 입력받을 수 있는 한 문장의 최대 글자 수
#define MAX_PTR_SIZE 100000		// CSV 파일에서 읽어들일 수 있는 최대 단락 수

char g_cszBuff[MAX_WORD_LENGTH];	// 입력 버퍼
wchar_t g_cszWBuff[MAX_WORD_LENGTH];	// 입력 버퍼


namespace FileController
{
	int FileRead(const char* FileName, const char* Mode, char*** outBuffer, int* outBuffer_size)
	{
		FILE* fp = NULL;
		fopen_s(&fp, FileName, Mode);
		if (fp == NULL)
		{
			ConsoleRenderer::print((char*)"File Dose not Opened.\n");
			return 0;
		}

		*outBuffer = (char**)malloc(sizeof(char*) * (MAX_BUFFER_POOL_SIZE + 1));

		int m_ioutBufferIndex = 0;

		while (true)
		{
			fgets(g_cszBuff, MAX_WORD_LENGTH, fp);
			if (feof(fp)) break;

			int g_cszBuff_size = strlen(g_cszBuff);
			(*outBuffer)[m_ioutBufferIndex] = (char*)malloc(sizeof(char) * (g_cszBuff_size + 1));

			for (int i = 0; i < g_cszBuff_size; i++)
				(*outBuffer)[m_ioutBufferIndex][i] = g_cszBuff[i];
			(*outBuffer)[m_ioutBufferIndex][g_cszBuff_size] = NULL;

			m_ioutBufferIndex++;
		}

		*outBuffer_size = m_ioutBufferIndex;

		fclose(fp);

		return 1;
	}

	int FileRead(const char* FileName, const char* Mode, wchar_t*** outBuffer, int* outBuffer_size)
	{
		FILE* fp = NULL;
		fopen_s(&fp, FileName, Mode);
		if (fp == NULL)
		{
			ConsoleRenderer::print((char*)"File Dose not Opened.\n");
			return 0;
		}

		*outBuffer = (wchar_t**)malloc(sizeof(wchar_t*) * (MAX_BUFFER_POOL_SIZE + 1));

		int m_ioutBufferIndex = 0;

		while (true)
		{

			fgetws(g_cszWBuff, MAX_WORD_LENGTH, fp);
			if (feof(fp)) break;

			int g_cszBuff_size = wcslen(g_cszWBuff);
			(*outBuffer)[m_ioutBufferIndex] = (wchar_t*)malloc(sizeof(wchar_t) * (g_cszBuff_size + 1));

			for (int i = 0; i < g_cszBuff_size; i++)
				(*outBuffer)[m_ioutBufferIndex][i] = g_cszWBuff[i];
			(*outBuffer)[m_ioutBufferIndex][g_cszBuff_size] = NULL;

			m_ioutBufferIndex++;
		}

		*outBuffer_size = m_ioutBufferIndex;

		fclose(fp);

		return 1;
	}

	int FileReadFromCSV(const char* FileName, const char* Mode, UI::FGAMEDIALOG* dialog, int* dialogSize)
	{
		FILE* fp = NULL;
		fopen_s(&fp, FileName, Mode);

		if (fp == NULL)
		{
			ConsoleRenderer::print((char*)"FileReadFromCSV: File Dose not Opened.\n");
			return 0;
		}

		int m_ioutBufferIndex = 0;

		while (true)
		{
			fgets(g_cszBuff, MAX_WORD_LENGTH, fp);
			if (feof(fp)) break;

			int g_cszBuff_size = strlen(g_cszBuff);
			char* context = NULL;
			char* ptr = strtok_s(g_cszBuff, ",", &context);
			char** ptrs = (char**)malloc(sizeof(char*)* MAX_PTR_SIZE);
			int count = 0;
			
			while (ptr != NULL)
			{
				int ptrSize = strlen(ptr) + 1;
				ptrs[count] = (char*)malloc(sizeof(char) * ptrSize);
				memcpy_s(ptrs[count++], ptrSize ,ptr, ptrSize);
				ConsoleRenderer::print(ptr);
				ptr = strtok_s(NULL, ",", &context);
			}

			dialog[m_ioutBufferIndex].InSize = atoi(ptrs[0]);
			dialog[m_ioutBufferIndex].Speaker.m_pcontent = ptrs[1];
			dialog[m_ioutBufferIndex].Speaker.m_icontentSize = strlen(ptrs[1]);
			dialog[m_ioutBufferIndex].Dialog.m_pcontent = ptrs[2];
			dialog[m_ioutBufferIndex].Dialog.m_icontentSize = strlen(ptrs[2]);
			dialog[m_ioutBufferIndex].Type.m_pcontent = ptrs[3];
			dialog[m_ioutBufferIndex].Type.m_icontentSize = strlen(ptrs[3]);
			dialog[m_ioutBufferIndex].Answer.m_pcontent = ptrs[4];
			dialog[m_ioutBufferIndex].Answer.m_icontentSize = strlen(ptrs[4]);
			dialog[m_ioutBufferIndex].Likeability.m_pcontent = ptrs[5];
			dialog[m_ioutBufferIndex].Likeability.m_icontentSize = strlen(ptrs[5]);

			m_ioutBufferIndex++;
		}

		*dialogSize = m_ioutBufferIndex;

		fclose(fp);

		return 1;
	}
}

