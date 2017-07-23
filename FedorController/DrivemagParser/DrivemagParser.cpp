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

const int DrivemagParser::frameDt = 5;
const bool DrivemagParser::isInterpolation = true;
const int DrivemagParser::minInterpolationDt = 1000;

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
void DrivemagParser::PlayDrivemag(string filename, function<void(double, map<string, double>)> callback)
{
	if (!experimental::filesystem::exists(filename))
		throw invalid_argument("File not found");

	if (callback == nullptr)
		throw invalid_argument("Invalid callback");

	ifstream drivemag(filename);
	if (!drivemag.is_open())
		throw runtime_error("Can't open file " + filename);

	//--- Считываемые значения ---
	int motor;
	double time = 0, pos;
	string wtf;				//Чтобы считать дичь


	//--- Для воспроизведения ---

	// Время из предыдущей строки. Сравнивается с текущим,
	// чтобы определить начало нового времени
	double lastTime = 0;

	// Время предыдущего фрейма
	double previousFrameTime = 0;

	// Два буфера поз, текущая и предыдущая, для 
	// интерполяции. Они переключаются и текущая и 
	// предыдущая меняется местами
	map<string, double> poses[2];
	map<string, double> interpolatedPoses;

	// Индекс текущей позы
	int curReadingPose = 0;

	// Индекс последней полностью считанной позы
	int readyPose = 0;

	// Счетчик фреймов
	// Чтобы начать обработку когда накопится 2 фрейма
	// (начало и конец интерполяции)
	int currentFrame = 0;

	// Кадр полностью считаны
	bool isFrameReady = false;

	// Для завершения цикла после чтения последней строки
	bool eof = false;

	// Начальное время
	auto t0 = chrono::steady_clock::now();


	// --- Чтение и воспроизведение ---

	while (true)
	{
		// Считать строку из файла
		if (!drivemag.eof())
		{
			drivemag >> motor >> time >> pos;
			getline(drivemag, wtf);

			//time *= 5000;

			// Начался новый фрейм, старый закончился
			if (time > lastTime)
			{
				readyPose = curReadingPose;
				isFrameReady = true;
				currentFrame++;
				SwapPose(curReadingPose);
			}
		}

		// Выполнение фрейма
		if (isFrameReady)
		{
			if (currentFrame > 1)
			{
				int dT = (lastTime - previousFrameTime) * 1000;		// Между последним и предпоследним завершенными фреймами

				auto & fromPose = poses[abs(readyPose - 1)];			// Из какой позиции 
				auto & toPose = poses[readyPose];						// В какую переходить


				//std::cout << std::fixed << std::setprecision(4)<< lastTime << "\n";

				if (isInterpolation && dT >= minInterpolationDt)
				{
					// Интерполяция
					Interpolate(fromPose, toPose, t0, dT, callback);
				}
				else
				{
					if (currentFrame == 2)
					{
						// Если без интерполяции выполняется переход между
						// фреймами 1 и 2, то надо установить оба с соотв. задержками.
						// В дальнейшем, только фрейм "to"

						RunSingle(fromPose, t0, dT, callback);
					}

					// Сразу устанавливаем в последний фрейм
					RunSingle(toPose,t0, dT, callback);

				}

				if (eof)
					break;

				previousFrameTime = lastTime;
			}

			isFrameReady = false;
		}

		AddDrive(poses[curReadingPose], motor, pos);
		lastTime = time;

		// Если добавлена последняя строка из файла - фрейм завершен
		// Цикл должен пройти еще раз, чтобы обработать фрейм
		if (drivemag.eof())
		{
			readyPose = curReadingPose;
			isFrameReady = true;
			currentFrame++;
			eof = true;
		}
	}


	drivemag.close();
}


// Переключает позы
void DrivemagParser::SwapPose(int & curPose)
{
	curPose++;
	if (curPose == 2)
		curPose = 0;
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
		pose[fedorDrive] = deg;
	}

}

//Воспроизводит одну позу
void DrivemagParser::RunSingle(map<string, double> pose, chrono::time_point<chrono::steady_clock> t0, int delay, function<void(double, map<string, double>)> callback)
{	
	int t = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - t0).count();

	callback(t, pose);
	Sleep(delay);
}

//Производит интерполяцию
void DrivemagParser::Interpolate(map<string, double> fromPose, map<string, double> toPose, chrono::time_point<chrono::steady_clock> t0, int delay, function<void(double, map<string, double>)> callback)
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
			double interpolated = (to - from) * t / (double)delay +from;

			interpolatedPoses.insert({ m.first, interpolated });
		}

		i++;
		if (i == 2)
			i = 0;

		RunSingle(interpolatedPoses, t0, frameDt, callback);
		interpolatedPoses.clear();


	} while (t < delay);	
}

//Переводит радианы в градусы
double DrivemagParser::rad2deg(double rad)
{
	return rad / M_PI * 180.0;
}