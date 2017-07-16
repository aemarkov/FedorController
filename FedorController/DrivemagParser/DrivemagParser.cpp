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
void DrivemagParser::PlayDrivemag(string filename, function<void(double, double, map<string, double>)> callback)
{
	if (!experimental::filesystem::exists(filename))
		throw invalid_argument("File not found");

	if (callback == nullptr)
		throw invalid_argument("Invalid callback");

	ifstream drivemag(filename);
	if (!drivemag.is_open())
		throw runtime_error("Can't open file " + filename);

	int motor;
	double time, pos;
	string wtf;				//Чтобы считать дичь

	double lastTime = 0;
	map<string, double> poses;

	while (!drivemag.eof())
	{

		drivemag >> motor >> time >> pos;
		getline(drivemag, wtf);

		// Считаны весь фрейм
		if (time > lastTime)
			ProcessFrame(lastTime, time, poses, callback);


		string fedorDrive = MapDrive(motor);
		if (fedorDrive != "")
		{
			double deg = InvertDrive(motor, rad2deg(pos));
			//double deg = InvertDrive(motor, pos);
			poses.insert(pair<string, double>(fedorDrive, deg));
		}

		lastTime = time;
	}

	ProcessFrame(lastTime, time, poses, callback);

	drivemag.close();
}

void DrivemagParser::ProcessFrame(double lastTime, double time, map<string, double> & poses, function<void(double, double, map<string,double>)> callback)
{
	int sleepTime = (time - lastTime) * 1000;
	callback(lastTime, sleepTime, poses);
	poses.clear();
	//Sleep(sleepTime);
}


//Преобразует номер двигателя из Drivemag в название мотора Федра
string DrivemagParser::MapDrive(int drive)
{
	map<int, string>::iterator it;

	if ((it = driveMap.find(drive)) == driveMap.end())
		return "";

	return it->second;
}

//Инвертирует ось
double DrivemagParser::InvertDrive(int drive, double angle)
{
	map<int, bool>::iterator it;

	if ((it = driveInvert.find(drive)) == driveInvert.end())
		return angle;

	return it->second ? -angle : angle;
}

//Переводит радианы в градусы
double DrivemagParser::rad2deg(double rad)
{
	return rad / M_PI * 180.0;
} 