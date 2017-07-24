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
	{ 20, false },
	{ 21, false },
	{ 35, false },
	{ 36, false },
	{ 37, false },
	{ 49, true }
};


DrivemagParser::DrivemagParser(bool isInterpolation, int frameDt, int minInterpolationDt)
{
	_frameDt = frameDt;
	_isInterpolation = isInterpolation;
	_minInterpolationDt = minInterpolationDt;
}

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
void DrivemagParser::LoadDrivemag(string filename)
{
	if (!experimental::filesystem::exists(filename))
		throw invalid_argument("File not found");

	ifstream drivemag(filename);
	if (!drivemag.is_open())
		throw runtime_error("Can't open file " + filename);

	//--- Считываемые значения ---
	int motor;
	double time = 0, pos;
	string wtf;				//Чтобы считать дичь


	double lastTime = 0;
	Frame frame;

	// --- Считывание файла ---
	while (!drivemag.eof())
	{
		drivemag >> motor >> time >> pos;
		getline(drivemag, wtf);

		if (time > lastTime)
		{
			// Начался другой кадр, предыдущий завершился
			frame.Time = lastTime;
			_frames.push_back(frame);
			frame.Pose.clear();
		}

		AddDrive(frame.Pose, motor, pos);

		if (drivemag.eof())
		{
			//Последний кадр завершился
			frame.Time = lastTime;
			_frames.push_back(frame);
		}

		lastTime = time;
	}

	drivemag.close();
}



// Плавно переходит в начальный фрейм
void FedorControl::DrivemagParser::ToStart(map<string, double> currentPose, double time, CallbackType callback)
{
	if (callback == nullptr)
		throw invalid_argument("Invalid callback");

	_t0 = chrono::steady_clock::now();
	Interpolate(currentPose, _frames[0].Pose, time * 1000.0, callback);
}



// Воспроизводит считанный файл Drivemag
void FedorControl::DrivemagParser::PlayDrivemag(CallbackType callback)
{
	if (callback == nullptr)
		throw invalid_argument("Invalid callback");

	_t0 = chrono::steady_clock::now();

	for (int curFrame = 0; curFrame < _frames.size() - 1; curFrame++)
	{
		Frame & fromFrame = _frames[curFrame];
		Frame & toFrame = _frames[curFrame + 1];

		int dT = (toFrame.Time - fromFrame.Time) * 1000;

		if (dT > _minInterpolationDt && _isInterpolation)
		{
			//Интерполяция
			Interpolate(fromFrame.Pose, toFrame.Pose, dT, callback);
		}
		else
		{
			// Мгновенное выполнение
			// Для первого кадра надо выполнить два - "to" и
			// "from", а дальше только "from", потому что "to" уже
			// выполнен на предыдущей итерации

			if (curFrame == 0)
			{
				RunSingle(fromFrame.Pose, dT, callback);
			}

			RunSingle(toFrame.Pose, dT, callback);
		}
	}
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

//Воспроизводит одну позу
void DrivemagParser::RunSingle(map<string, double>& pose, int delay, CallbackType callback)
{
	int t = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - _t0).count();

	callback(t, pose);
	Sleep(delay);
}

//Производит интерполяцию
void DrivemagParser::Interpolate(map<string, double> & fromPose, map<string, double> & toPose, int delay, CallbackType callback)
{

	map<string, double> interpolatedPoses;
	auto interpolationT0 = chrono::steady_clock::now();
	double t;
	int i = 0;

	do
	{
		t = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - interpolationT0).count();

		for (auto & m : fromPose)
		{
			double from = m.second;
			double to = toPose[m.first];
			double interpolated = (to - from) * t / (double)delay + from;

			interpolatedPoses.insert({ m.first, interpolated });
		}

		i++;
		if (i == 2)
			i = 0;

		RunSingle(interpolatedPoses, _frameDt, callback);
		interpolatedPoses.clear();


	} while (t < delay);
}

//Переводит радианы в градусы
double DrivemagParser::rad2deg(double rad)
{
	return rad / M_PI * 180.0;
}