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
#include <FedorClock.h>
#include "Drivemag.h"

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
			Парсит файл формата DRIVEMAG.
			/param[in] filename Имя файла drivemag
		*/
		static vector<Frame> LoadDrivemag(string filename, bool isDegrees=false);

	private:

		static map<int, string> driveMap;							// Мапит названия двигателей
		static map<int, bool> driveInvert;							// Какие оси надо инвертировать


		//Преобразует номер двигателя из Drivemag в название мотора Федра
		static string MapDrive(int drive);

		//Инвертирует ось
		static double InvertDrive(int drive, double angle);

		//Добавляет мотор
		static void AddDrive(map<string, double> & pose, int motor, double pos, bool isDegrees);

		//Переводит радианы в градусы
		static double rad2deg(double rad);
	};
}

#endif

