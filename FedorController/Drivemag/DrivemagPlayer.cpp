#include "DrivemagPlayer.h"

using namespace FedorControl;

DrivemagPlayer::DrivemagPlayer(bool isInterpolation, int frameDt, int minInterpolationDt, FedorClock& clock)
	: _clock(clock)
{
	_frameDt = frameDt;
	_isInterpolation = isInterpolation;
	_minInterpolationDt = minInterpolationDt;

	_hTimer = CreateWaitableTimer(NULL, TRUE, NULL);
}

DrivemagPlayer::~DrivemagPlayer()
{
	CloseHandle(_hTimer);
}

void DrivemagPlayer::ToStart(const Pose& currentPose, const Pose& toPose, const double time, const CallbackType callback)
{
	if (callback == nullptr)
		throw invalid_argument("Invalid callback");

	setTimer(0);
	Interpolate(currentPose, toPose, time * 1000.0, callback);
}

void DrivemagPlayer::PlayDrivemag(const vector<Frame>& frames, const CallbackType callback)
{
	if (callback == nullptr)
		throw invalid_argument("Invalid callback");

	setTimer(0);

	for (int curFrame = 0; curFrame < frames.size() - 1; curFrame++)
	{
		const Frame & fromFrame = frames[curFrame];
		const Frame & toFrame = frames[curFrame + 1];

		int dT = (toFrame.Time - fromFrame.Time) * 1000;

		if (dT > _minInterpolationDt && _isInterpolation)
		{
			//Интерполяция
			Interpolate(fromFrame.Pose, toFrame.Pose, dT, callback);
		}
		else
		{
			// Мгновенное выполнение
			// Для первого кадра надо выполнить два - "to" и
			// "from", а дальше только "to", потому что "from" уже
			// выполнен на предыдущей итерации

			if (curFrame == 0)
			{
				RunSingle(fromFrame.Pose, dT, fromFrame.Time, callback);
			}

			RunSingle(toFrame.Pose, dT, toFrame.Time, callback);
		}
	}
}



//Воспроизводит одну позу
void DrivemagPlayer::RunSingle(const Pose& pose, const int delay, const double origTime, const CallbackType callback)
{	
	WaitForSingleObject(_hTimer, INFINITE);
	setTimer(delay);

	auto t = _clock.GetTime();
	//callback(chrono::duration<float>(origTime), pose);		// Для записи времен из драйвмага
	callback(t, pose);
}

//Производит интерполяцию
void DrivemagPlayer::Interpolate(const Pose & fromPose, const Pose& toPose, const int delay, const CallbackType callback)
{

	map<string, double> interpolatedPoses;
	const auto interpolationT0 = chrono::steady_clock::now();
	double t;
	int i = 0;

	do
	{
		t = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - interpolationT0).count();

		for (auto & m : fromPose)
		{
			const double from = m.second;

			if (toPose.find(m.first) == toPose.end())
				continue;

			const double to = toPose.at(m.first);
			double interpolated = (to - from) * t / (double)delay + from;

			interpolatedPoses.insert({ m.first, interpolated });
		}

		i++;
		if (i == 2)
			i = 0;

		// Тут вместо настоящего времени из драйвмага передаем реальное.
		// Щито поделать
		RunSingle(interpolatedPoses, _frameDt, t / 1000.0, callback);
		interpolatedPoses.clear();


	} while (t < delay);
}

// Устанавливает задержку таймера (мс)
void DrivemagPlayer::setTimer(const int delay)
{
	LARGE_INTEGER lDelay;
	lDelay.QuadPart = -delay * 10000;
	SetWaitableTimer(_hTimer, &lDelay, 0, NULL, NULL, FALSE);
}