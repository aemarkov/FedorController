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


namespace FedorControl
{
	using namespace std;

	/**
	Парсит и воспроизводит файлы формата DRIVEMAG.
	*/
	class DrivemagParser
	{
	public:

		/**
		Парсит и воспроизводит файл формата DRIVEMAG.
		Эта функция блокирующая

		/param[in] filename Имя файла drivemag
		/param[in] callback функция, вызываемая каждый кадр
		*/
		static void PlayDrivemag(string filename, function<void(double, map<string, double>)> callback);

	private:

		static map<int, string> driveMap;			// Мапит названия двигателей
		static map<int, bool> driveInvert;			// Какие оси надо инвертировать

		static const int frameDt;					// Желаемая длительность фрейма для интерполяции (мс)
		static const bool isInterpolation;
		static const int minInterpolationDt;

		//Преобразует номер двигателя из Drivemag в название мотора Федра
		static string MapDrive(int drive);

		//Инвертирует ось
		static double InvertDrive(int drive, double angle);

		//Добавляет мотор
		static void AddDrive(map<string, double> & pose, int motor, double pos);

		//Воспроизводит одну позу
		static void RunSingle(map<string, double> poses, chrono::time_point<chrono::steady_clock> t0, int delay, function<void(double, map<string, double>)> callback);

		//Выполняет интерполяцию
		static void Interpolate(map<string, double> fromPose, map<string, double> toPose, chrono::time_point<chrono::steady_clock> t0, int delay, function<void(double, map<string, double>)> callback);

		//Переводит радианы в градусы
		static double rad2deg(double rad);

		// Переключает позы
		static void SwapPose(int & curPose);

		
	};
}

#endif

