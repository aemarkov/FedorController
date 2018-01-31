#pragma once
#include <string>
#include <map>
#include <chrono>
#include <functional>
#include <map>
#include <vector>
#include <Windows.h>
#include "FedorClock.h"
#include "Frame.h"

#define _USE_MATH_DEFINES
#include <math.h>

namespace FedorControl
{
	class DrivemagPlayer
	{
	public:

		using CallbackType = std::function<void(std::chrono::duration<float>, std::map<std::string, double>)>;

		/**
		Создает новый парсер.
		/param[in] isInterpolation Интепрполировать ли значения
		/param[in] frameDt Шаг интерполяции
		/param[in] Минимальное время между двумя кадрами, при котором будет
		включена интерполяция, иначе - без интерполяции
		*/
		DrivemagPlayer(bool isInterpolation = false, int frameDt = 5, int minInterpolationDt = 20, FedorClock& clock = FedorClock());
		~DrivemagPlayer();

		/**
		/brief Плавно переходит в начальный фрейм
		/param[in] Текущее положение приводов робот
		/param[in] Положение, в которое надо перевести робота
		/param[in] time Время, за которое надо перейти из текущего в начальную позу
		/param[in] callback Функция, восппроизводящая позу
		*/
		void ToStart(const Pose& currentPose, const Pose& toPose, const double time, const CallbackType callback);

		/**
		/brief Воспроизводит считанный файл Drivemag.

		Эта функция блокирующая и вернет управление, только
		когда весь файл будет воспроизведен

		/param[in] frames Кадры драйвмага
		/param[in] callback Функция, восппроизводящая позу
		*/
		void PlayDrivemag(const vector<Frame>& frames, const CallbackType callback);

	private:
		int _frameDt;												// Желаемая длительность фрейма для интерполяции (мс)
		bool _isInterpolation;
		int _minInterpolationDt;

		FedorClock& _clock;
		HANDLE _hTimer;

		//Воспроизводит одну позу
		void RunSingle(const Pose& pose, const int delay, const double origTime, const CallbackType callback);

		//Выполняет интерполяцию
		void Interpolate(const Pose& fromPose, const Pose& toPose, const int delay, const CallbackType callback);


		// Устанавливает задержку таймера (мс)
		void setTimer(int delay);
	};
}
