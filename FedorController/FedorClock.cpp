#include "FedorClock.h"

using namespace std;

FedorClock::FedorClock()
{
	ResetClock();
}

void FedorClock::ResetClock()
{
	_t0 = chrono::steady_clock::now();
}

std::chrono::duration<float> FedorClock::GetTime()
{
	return chrono::steady_clock::now() - _t0;
}
