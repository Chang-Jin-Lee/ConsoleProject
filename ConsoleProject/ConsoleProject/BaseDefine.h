#pragma once
#include "Time.h"

typedef enum ESceneState {
	MENU,
	ANIMATION,
	PLAY,
	END
} ESceneState;

#define VK_Q 0x41 + 'q' - 'a'
#define VK_W 0x41 + 'w' - 'a'
#define VK_E 0x41 + 'e' - 'a'
#define VK_R 0x41 + 'r' - 'a'
#define VK_T 0x41 + 't' - 'a'
#define VK_R 0x41 + 'r' - 'a'
#define VK_Z 0x41 + 'z' - 'a'
#define VK_X 0x41 + 'x' - 'a'
#define VK_C 0x41 + 'c' - 'a'

#define VK_0 0x30
#define VK_1 0x31
#define VK_2 0x32
#define VK_3 0x33
#define VK_4 0x34
#define VK_5 0x35
#define VK_6 0x36
#define VK_7 0x37
#define VK_8 0x38
#define VK_9 0x39 

// File Controller에서 쓰이는 것 
#define MAX_BUFFER_POOL_SIZE 2000	// 버퍼의 최대 풀 개수 (y 축으로 몇 줄인지.)
#define MAX_WORD_LENGTH 2000				// 단어의 최대 알파벳 숫자는 14개, 끝은 문자 NULL (값0)으로 채워야한다.== txt 파일로 입력받을 수 있는 한 문장의 최대 글자 수
#define MAX_PTR_SIZE 3000		// CSV 파일에서 읽어들일 수 있는 최대 단락 수


// 대사 게임에서 쓰이는 것
#define MAX_DIALOG_SIZE 100
#define MAX_VIDEO_SIZE 8000


// 커서로 메뉴를 선택할 때 쓰이는 것
#define MAX_PERCENT_STRING_SIZE 40	// 퍼센트 게이지바 칸 개수
#define MAX_BUTTON_COUNT 2			// 메뉴 셀렉트 개수

// Animation Scene 대사에서 쓰이는 것.
#define MAX_CHARACTER_SIZE 4	// 대사에 등장하는 최대 캐릭터 수
#define MAX_SELECTBUBBLE_SIZE 2	// 선택창 개수