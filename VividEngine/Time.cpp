#include "stdafx.h"
#include "Time.h"
#include "Debug.h"

float Time::deltaTime = 0;
double Time::frequency = 0;
__int64 Time::startTime = 0;
double Time::lastTime = 0;

void Time::Initialize()
{
	LARGE_INTEGER largeInteger;
	if(!QueryPerformanceFrequency(&largeInteger))
		vivid::Debug::Log("System doesn't support high performance timer!");

	frequency = double(largeInteger.QuadPart) / 1000;

	QueryPerformanceCounter(&largeInteger);
	startTime = largeInteger.QuadPart;
}

double Time::TotalTime()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return double(li.QuadPart - startTime) / frequency;
}

void Time::Record()
{
	lastTime = TotalTime();
}

double Time::Elapsed()
{
	return TotalTime() - lastTime;
}