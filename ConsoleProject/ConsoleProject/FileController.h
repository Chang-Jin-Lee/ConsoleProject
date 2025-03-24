#pragma once


namespace FileController
{
	int FileRead(const char* FileName, const char* Mode, char*** outBuffer, int* outBuffer_size);	// ret : outBuffer의 크기
	int FileRead(const char* FileName, const char* Mode, wchar_t*** outBuffer, int* outBuffer_size);	// ret : outBuffer의 크기
}