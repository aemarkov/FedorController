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
#include <Windows.h>

#include <iostream>
#include <iomanip>

namespace FedorControl
{
	/**
	Парсит и воспроизводит файлы формата DRIVEMAG.
	*/
	class DrivemagParser
	{
	public:

		using RobotPos = std::map<std::string, double>;

		/**
		Парсит и воспроизводит файл формата DRIVEMAG.
		Эта функция блокирующая

		/param[in] filename Имя файла drivemag
		/param[in] callback функция, вызываемая каждый кадр
		*/
		static void PlayDrivemag(std::string filename, std::function<void(RobotPos)> callback);

	private:

		//Преобразует номер двигателя из Drivemag в название мотора Федра
		static std::string MapDrive(int drive);

		//Инвертирует ось
		static double InvertDrive(int drive, double angle);

		//Переводит радианы в градусы
		static double rad2deg(double rad);

		static std::map<int, std::string> driveMap;			// Мапит названия двигателей
		static std::map<int, bool> driveInvert;				// Какие оси надо инвертировать


		static void Log(int line, double time, std::ofstream & log, RobotPos & poses);
	};
}

#endif

