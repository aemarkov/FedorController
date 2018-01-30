#pragma once

#include <chrono>

// Используется для единообразного отсчета времени
class FedorClock
{
public:
	FedorClock();
	void ResetClock();	
	std::chrono::duration<float> GetTime();

private:
	std::chrono::time_point<std::chrono::steady_clock> _t0;
};
