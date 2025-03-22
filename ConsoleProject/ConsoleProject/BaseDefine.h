#pragma once
#include "Time.h"

typedef enum class ESceneState {
	MENU,
	PLAY,
	END,
	MAX
};

#define VK_Q 0x41 + 'q' - 'a'
#define VK_W 0x41 + 'w' - 'a'
#define VK_E 0x41 + 'e' - 'a'