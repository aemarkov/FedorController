#include "DrivemagParser.h"

using namespace std;
using namespace FedorControl;


map<int, string> DrivemagParser::driveMap = {
	{ 1, "R.AnkleS" },
	{ 2, "R.AnkleF" },
	{ 3, "R.Knee" },
	{ 4, "R.HipF" },
	{ 5, "R.HipS" },
	{ 6, "R.HipR" },
	{ 7, "L.AnkleS" },
	{ 8, "L.AnkleF" },
	{ 9, "L.Knee" },
	{ 10, "L.HipF" },
	{ 11, "L.HipS" },
	{ 12, "L.HipR" },
	{ 19, "R.ShoulderS" },
	{ 20, "R.ShoulderF" },
	{ 21, "R.Elbow" },
	{ 35, "L.ShoulderS" },
	{ 36, "L.ShoulderF" },
	{ 37, "L.Elbow" },
	{ 49, "TorsoR" }
};


map<int, bool> DrivemagParser::driveInvert = {
	{ 1, true },
	{ 2, true },
	{ 3, true },
	{ 4, true },
	{ 5, true },
	{ 6, false },
	{ 7, false },
	{ 8, true },
	{ 9, true },
	{ 10, true },
	{ 11, true },
	{ 12, true },
	{ 19, true },
	{ 20, true },
	{ 21, false },
	{ 35, false },
	{ 36, true },
	{ 37, false },
	{ 49, true }
};



//Парсит файлы формата DRIVEMAG
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
vector<Frame> DrivemagParser::LoadDrivemag(string filename)
{
	cout << "Reading " << filename << "..." << endl;

	vector<Frame> drivemag;

	if (!experimental::filesystem::exists(filename))
		throw invalid_argument("File not found");

	ifstream file(filename);
	if (!file.is_open())
		throw runtime_error("Can't open file " + filename);

	//--- Считываемые значения ---
	int motor;
	double time = 0, pos;
	string wtf;				//Чтобы считать дичь


	double lastTime = 0;
	Frame frame;

	// --- Считывание файла ---
	while (!file.eof())
	{
		file >> motor >> time >> pos;
		getline(file, wtf);

		if (time > lastTime)
		{
			// Начался другой кадр, предыдущий завершился
			frame.Time = lastTime;
			drivemag.push_back(frame);
			frame.Pose.clear();
		}

		AddDrive(frame.Pose, motor, pos);

		if (file.eof())
		{
			//Последний кадр завершился
			frame.Time = lastTime;
			drivemag.push_back(frame);
		}

		lastTime = time;
	}

	file.close();

	return move(drivemag);
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

//Добавляет мотор
void DrivemagParser::AddDrive(map<string, double> & pose, int motor, double pos)
{
	// Добавляем мотор в текущий буфер
	string fedorDrive = MapDrive(motor);
	if (fedorDrive != "")
	{
		double deg = InvertDrive(motor, rad2deg(pos));
		//double deg = InvertDrive(motor, pos);
		pose.insert(pair<string, double>(fedorDrive, deg));
	}

}


//Переводит радианы в градусы
double DrivemagParser::rad2deg(double rad)
{
	return rad / M_PI * 180.0;
}
