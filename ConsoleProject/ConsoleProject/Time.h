#pragma once
#include <Windows.h>

class Time
{
public:
	Time() {};
	~Time() {};

private:
	LARGE_INTEGER frequency;
	LARGE_INTEGER prevCounter;
	LARGE_INTEGER currentCounter;
	LARGE_INTEGER initCounter;
	float deltaTime = 0;

public:
	void Initialize()
	{
		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&initCounter);
	}
	void UpdateTime()
	{
		QueryPerformanceCounter(&currentCounter);
		deltaTime = static_cast<float>(currentCounter.QuadPart - prevCounter.QuadPart) / frequency.QuadPart;
		prevCounter = currentCounter;
	}
	float GetElapsedTime()
	{
		return deltaTime;
	}
	float GetTotalTime()
	{
		float totalTime = static_cast<float>(currentCounter.QuadPart - initCounter.QuadPart) / frequency.QuadPart;
		return totalTime;
	}
};