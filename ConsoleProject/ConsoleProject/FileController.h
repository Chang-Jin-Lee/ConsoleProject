#pragma once
#include "BaseDefine.h"
#include "UI.h"
#include "Object.h"

namespace FileController
{
	int FileRead(const char* FileName, const char* Mode, char*** outBuffer, int* outBuffer_size);	// ret : outBuffer의 크기
	int FileRead(const char* FileName, const char* Mode, wchar_t*** outBuffer, int* outBuffer_size);	// ret : outBuffer의 크기
	int FileReadFromCSV(const char* FileName, const char* Mode, UI::FGAMEDIALOG* dialog, int* dialogSize);	// ret : outBuffer의 크기
	int FileReadVideo(const char* VideoName, UI::FVideo* video); //비디오를 읽어와서 넣어주기 ex) Aru_frame_0001 라면 처음 Aru만 넣으면 됨.
	int FileReadAnimation(const char* AnimationName, const int& animationState, Object::FPlayerCharacter* pc); //비디오를 읽어와서 넣어주기 ex) Aru_frame_0001 라면 처음 Aru만 넣으면 됨.
}