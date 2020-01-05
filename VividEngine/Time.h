#pragma once

class Time {
public:
	Time() {}
	Time(const Time&) {}
	~Time();

	static void Initialize();
	static double TotalTime();
	static void Record();
	static double Elapsed();

	static float deltaTime;
private:
	static double lastTime;
	static double frequency;
	static __int64 startTime;
};