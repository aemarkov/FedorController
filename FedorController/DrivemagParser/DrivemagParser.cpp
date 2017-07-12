#include "DrivemagParser.h"

using namespace std;
using namespace FedorControl;

 map<int, string> DrivemagParser::drivemap = {
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
		{
			callback(poses);

			poses.clear();
			int sleepTime = (time - lastTime) * 1000;
			Sleep(sleepTime);
		}


		string fedorDrive = MapDrive(motor);
		if (fedorDrive != "")
			poses.insert(std::pair<string, double>(fedorDrive, rad2deg(pos)));

		lastTime = time;
	}

	drivemag.close();
}


//Преобразует номер двигателя из Drivemag в название мотора Федра
string DrivemagParser::MapDrive(int drive)
{
	std::map<int, string>::iterator it;

	if ((it = drivemap.find(drive)) == drivemap.end())
		return "";

	return it->second;
}

//Переводит радианы в градусы
double DrivemagParser::rad2deg(double rad)
{
	return rad / M_PI * 180.0;
}