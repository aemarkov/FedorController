#ifndef __DRIVEMAGPARSER_H__
#define __DRIVEMAGPARSER_H__




#include <fstream>
#include <string>
#include <functional>
#include <map>
#include <chrono>
#include <filesystem>
#include <exception>

#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream>
#include <iomanip>
#include <chrono>
#include <Windows.h>

#include "Frame.h"

namespace FedorControl
{
	using namespace std;

	/**
	Парсит и воспроизводит файлы формата DRIVEMAG.
	*/
	class DrivemagParser
	{
	public:

		using CallbackType = function<void(double, map<string, double>)>;

		/**
			Создает новый парсер.
			/param[in] isInterpolation Интепрполировать ли значения
			/param[in] frameDt Шаг интерполяции
			/param[in] Минимальное время между двумя кадрами, при котором будет
			           включена интерполяция, иначе - без интерполяции
		*/
		DrivemagParser(bool isInterpolation = false, int frameDt = 5, int minInterpolationDt = 20);

		/**
			Парсит файл формата DRIVEMAG.
			/param[in] filename Имя файла drivemag
		*/
		void LoadDrivemag(string filename);

		/**
			/brief Плавно переходит в начальный фрейм
			/param[in] Текущее положение приводов робота
			/param[in] time Время, за которое надо перейти из текущего в начальную позу
			/param[in] callback Функция, восппроизводящая позу
		*/
		void ToStart(map<string, double> currentPose, double time, CallbackType callback);

		/**
			/brief Воспроизводит считанный файл Drivemag.

			Эта функция блокирующая и вернет управление, только
			когда весь файл будет воспроизведен

			/param[in] callback Функция, восппроизводящая позу
		*/
		void PlayDrivemag(CallbackType callback);

	private:

		static map<int, string> driveMap;							// Мапит названия двигателей
		static map<int, bool> driveInvert;							// Какие оси надо инвертировать

		int _frameDt;												// Желаемая длительность фрейма для интерполяции (мс)
		bool _isInterpolation;
		int _minInterpolationDt;
		vector<Frame> _frames;										// Список кадров
		chrono::time_point<chrono::steady_clock> _t0;				// Время начала выполнения

		//Преобразует номер двигателя из Drivemag в название мотора Федра
		string MapDrive(int drive);

		//Инвертирует ось
		double InvertDrive(int drive, double angle);

		//Добавляет мотор
		void AddDrive(map<string, double> & pose, int motor, double pos);

		//Воспроизводит одну позу
		void RunSingle(map<string, double> & poses,  int delay, CallbackType callback);

		//Выполняет интерполяцию
		void Interpolate(map<string, double> & fromPose, map<string, double> & toPose, int delay, CallbackType callback);

		//Переводит радианы в градусы
		double rad2deg(double rad);
		
	};
}

#endif

