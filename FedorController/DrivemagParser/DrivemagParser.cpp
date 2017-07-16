#include "DrivemagParser.h"

using namespace std;
using namespace FedorControl;

map<int, string> DrivemagParser::driveMap = {
	{ 1, "R.AnkleF" },
	{ 2, "R.AnkleS" },
	{ 3, "R.Knee" },
	{ 4, "R.HipS" },
	{ 5, "R.HipF" },
	{ 6, "R.HipR" },
	{ 7, "L.AnkleF" },
	{ 8, "L.AnkleS" },
	{ 9, "L.Knee" },
	{ 10, "L.HipS" },
	{ 11, "L.HipF" },
	{ 12, "L.HipR" },
	{ 19, "R.ShoulderF" },
	{ 20, "R.ShoulderS" },
	{ 35, "L.ShoulderF" },
	{ 36, "L.ShoulderS" },
	{ 49, "TorsoR" }
};

map<int, bool> DrivemagParser::driveInvert = {
	{ 1, true },
	{ 2, true },
	{ 3, true },
	{ 4, true },
	{ 5, true },
	{ 6, false },
	{ 7, true },
	{ 8, false },
	{ 9, true },
	{ 10, false },
	{ 11, true },
	{ 12, true },
	{ 19, false },
	{ 20, true },
	{ 35, false },
	{ 36, false },
	{ 49, true }
};

//Парсит и воспроизводит файлы формата DRIVEMAG
/*
Формат файла
номер_привода_1 время_1 угол какая-то дичь дичь   \
номер_привода_2 время_1 угол какая-то дичь дичь   |   Фрейм 1
...                                               |
номер_привода_n время_1 угол какая-то дичь дичь   /
номер_привода_1 время_2 угол какая-то дичь дичь   \
номер_привода_2 время_2 угол какая-то дичь дичь   |   Фрейм 2
...                                               |
номер_привода_n время_2 угол какая-то дичь дичь   /
*/
void DrivemagParser::PlayDrivemag(string filename, function<void(RobotPos)> callback)
{
	if (!experimental::filesystem::exists(filename))
		throw std::invalid_argument("File not found");

	if (callback == nullptr)
		throw std::invalid_argument("Invalid callback");

	ifstream drivemag(filename);
	if (!drivemag.is_open())
		throw std::runtime_error("Can't open file " + filename);

	int line = 0;
	int motor;
	double time, pos;
	string wtf;				//Чтобы считать дичь

	double lastTime = 0;
	map<string, double> poses;

	ofstream log("log.csv");

	while (!drivemag.eof())
	{

		drivemag >> motor >> time >> pos;
		getline(drivemag, wtf);

		// Считаны весь фрейм
		if (time > lastTime)
		{
			Log(line, time, log, poses);

			callback(poses);

			poses.clear();
			int sleepTime = (time - lastTime) * 1000;
			Sleep(sleepTime);

			line++;
		}


		string fedorDrive = MapDrive(motor);
		if (fedorDrive != "")
		{
			double deg = InvertDrive(motor, pos);
			poses.insert(std::pair<string, double>(fedorDrive, deg));
		}

		lastTime = time;
	}

	log.close();
	drivemag.close();
}


//Преобразует номер двигателя из Drivemag в название мотора Федра
string DrivemagParser::MapDrive(int drive)
{
	std::map<int, string>::iterator it;

	if ((it = driveMap.find(drive)) == driveMap.end())
		return "";

	return it->second;
}

//Инвертирует ось
double DrivemagParser::InvertDrive(int drive, double angle)
{
	std::map<int, bool>::iterator it;

	if ((it = driveInvert.find(drive)) == driveInvert.end())
		return angle;

	return it->second ? -angle : angle;
}

//Переводит радианы в градусы
double DrivemagParser::rad2deg(double rad)
{
	return rad / M_PI * 180.0;
}

void DrivemagParser::Log(int line, double time, ofstream & log, RobotPos & poses)
{
	//Выводим заголовок
	if (line == 0)
	{
		std::cout << setw(8) << "Time" << " | ";
		log << "Time;";

		for (auto it = poses.begin(); it != poses.end(); ++it)
		{
			std::cout << fixed << setw(12) << it->first;
			log << it->first << ";";
		}

		std::cout << "\n";
		log << "\n";

	}

	std::cout << fixed << setw(8) << setprecision(4) << time << " | ";
	log << fixed << setprecision(4) << time << ";";
	for (auto it = poses.begin(); it != poses.end(); ++it)
	{
		std::cout << fixed << setw(12) << setprecision(2) << it->second;
		log << fixed << setprecision(4) << it->second << ";";
	}

	std::cout << "\n";
	log << "\n";
	log.flush();
}