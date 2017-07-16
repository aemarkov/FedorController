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
		static void PlayDrivemag(string filename, function<void(double, double, map<string, double>)> callback);

	private:

		static map<int, string> driveMap;			// Мапит названия двигателей
		static map<int, bool> driveInvert;				// Какие оси надо инвертировать



		//Преобразует номер двигателя из Drivemag в название мотора Федра
		static string MapDrive(int drive);

		//Инвертирует ось
		static double InvertDrive(int drive, double angle);

		//Переводит радианы в градусы
		static double rad2deg(double rad);

		static void ProcessFrame(double lastTime, double time, map<string, double> & poses, function<void(double, double, map<string, double>)> callback);
	};
}

#endif

