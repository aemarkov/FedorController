#include <WinSock2.h>
#include <Windows.h>


#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <chrono>

#include <Fedor\Fedor.h>
#include <Drivemag\DrivemagParser.h>
#include <Drivemag\DrivemagPlayer.h>
#include <PlotLogger\PlotLogger.h>
#include <FedorClock.h>

#include <filesystem>


using namespace FedorControl;
using namespace std;
using namespace SocketLib;

void PlayFrame(chrono::duration<float> t, map<string, double> & poses);
void PlayFrame_ToStart(chrono::duration<float> t, map<string, double> & poses);


void CreateLoggerHeader(Pose& pose);
void LogIMU();
void LogForces();
void LogStay(long milliseconds, Pose& stayingPose);

FedorClock fedorClock;
DrivemagPlayer player(true, 5, 20, fedorClock);
Fedor* fedor;
PlotLogger logger;

bool noLog = false;

int main(int argc, char** argv)
{
	if (argc < 4)
	{
		cout << "Usage:\n";
		cout << "FedorController <drivemag.txt> ip port [-nolog]\n";
		return -1;
	}

	if(argc == 5)
	{
		if(strcmp(argv[4], "-nolog")!=0)
		{
			cout << "Invalid argument " << argv[4] << "\n";
			return -1;
		}

		noLog = true;
	}

	char* drivemagFilename = argv[1];
	char* ip = argv[2];
	int port = atoi(argv[3]);


	Socket::Init();
	fedor = new Fedor();

	try
	{
		fedor->Connect(ip, port);
	}
	catch(exception ex)
	{
		cout << "Can't connect to F.E.D.O.R:\n";
		cout << ex.what() << "\n";
		return -1;
	}
	


	//auto dm1 = DrivemagParser::LoadDrivemag("D:\\Programming\\_PROJECTS\\Fedor\\FRUND\\31.01.2018\\DRIVEMAG1.TXT");
	auto dm2 = DrivemagParser::LoadDrivemag("D:\\Programming\\_PROJECTS\\Fedor\\FRUND\\31.01.2018\\DRIVEMAG2.TXT");

	if (!noLog)
	{		
		logger.Begin("D:\\Programming\\_PROJECTS\\Fedor\\FRUND\\31.01.2018\\log.txt", ' ');
		CreateLoggerHeader(dm2[0].Pose);
	}	


	fedor->ResetScene();
	Sleep(500);	
	fedor->Robot().Motors().Posset({ { "Neck", 90 },{ "HeadF", 90 } });
	fedorClock.ResetClock();

	cout << "Moving to start position...\n";
	player.ToStart(fedor->Robot().Motors().Posget(), dm2[0].Pose, 2, PlayFrame);

	//cout << "Playing DRIVEMAG 1...\n";
	//player.PlayDrivemag(dm1, PlayFrame);
	
	//cout << "Movingn 1 -> 2...\n";
	//player.ToStart(fedor->Robot().Motors().Posget(), dm2[0].Pose, 2, PlayFrame);

	cout << "Playing DRIVEMAG 2...\n";
	player.PlayDrivemag(dm2, PlayFrame);

	fedor->Disconnect();
	delete fedor;
	return 0;
}


// Записывает в лог неподвижного робота в течении заданного времени
void LogStay(long milliseconds, Pose& stayingnPose)
{
	auto t0 = chrono::steady_clock::now();
	do
	{
		float t = fedorClock.GetTime().count();
		logger.BeginFrame(t);

		auto realPoses = fedor->Robot().Motors().Posget();

		for (auto & pos : stayingnPose)
			logger.AddValue(pos.first, realPoses[pos.first]);

		for (auto & pose : realPoses)
			logger.AddValue(pose.first + "_real", pose.second);

		LogIMU();
		LogForces();


	} while (chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - t0).count() < milliseconds);
}

// Воспроизводит кадр и записывает лог
void PlayFrame(chrono::duration<float> t, Pose & pose)
{
	fedor->Robot().Motors().Posset(pose);

	if (!noLog)
	{
		float tt = t.count();
		logger.BeginFrame(tt);

		auto realPoses = fedor->Robot().Motors().Posget();

		for (auto & motor : pose)
			logger.AddValue(motor);		

		for (auto & motor : realPoses)		
			logger.AddValue(motor.first + "_real", motor.second);		

		LogIMU();
		LogForces();
	}
}

// Просто воспроизводит позы
void PlayFrame_ToStart(chrono::duration<float>  t, map<string, double> & poses)
{
	fedor->Robot().Motors().Posset(poses);
}


void CreateLoggerHeader(Pose& pose)
{
	/*
	Почему я инициализирую логгер тут?
	Да потому что мне надо знать, какие двигатели присутствуют
	в файле (pose)
	*/

	logger.AddTitle("Time");

	for (auto & pos : pose)
		logger.AddTitle(pos.first);

	auto motors = fedor->Robot().Motors().List();
	for (auto & motor : motors)
		logger.AddTitle(motor + "_real");

	logger.AddTitle("Yaw");
	logger.AddTitle("Pitch");
	logger.AddTitle("Roll");

	logger.AddTitle("R.FTS.Ankle_Fx");
	logger.AddTitle("R.FTS.Ankle_Fy");
	logger.AddTitle("R.FTS.Ankle_Fz");
	logger.AddTitle("R.FTS.Ankle_Tx");
	logger.AddTitle("R.FTS.Ankle_Ty");
	logger.AddTitle("R.FTS.Ankle_Tz");

	logger.AddTitle("L.FTS.Ankle_Fx");
	logger.AddTitle("L.FTS.Ankle_Fy");
	logger.AddTitle("L.FTS.Ankle_Fz");
	logger.AddTitle("L.FTS.Ankle_Tx");
	logger.AddTitle("L.FTS.Ankle_Ty");
	logger.AddTitle("L.FTS.Ankle_Tz");

	logger.AddTitle("ZMP.X");
	logger.AddTitle("ZMP.Y");

	logger.FinishHeader();
}

void LogIMU()
{
	auto imu = fedor->Robot().Sensors().GetIMU();
	logger.AddValue("Yaw", imu.Euler.x);
	logger.AddValue("Pitch", imu.Euler.y);
	logger.AddValue("Roll", imu.Euler.z);
}

void LogForces()
{
	auto force = fedor->Robot().Sensors().GetForce({ SensorsGroup::L_FTS_Ankle, SensorsGroup::R_FTS_Ankle });
	auto forceLeft = force[SensorsGroup::L_FTS_Ankle];
	auto forceRight = force[SensorsGroup::R_FTS_Ankle];

	logger.AddValue("R.FTS.Ankle_Fx", forceRight.F.x);
	logger.AddValue("R.FTS.Ankle_Fy", forceRight.F.y);
	logger.AddValue("R.FTS.Ankle_Fz", forceRight.F.z);
	logger.AddValue("R.FTS.Ankle_Tx", forceRight.T.x);
	logger.AddValue("R.FTS.Ankle_Ty", forceRight.T.y);
	logger.AddValue("R.FTS.Ankle_Tz", forceRight.T.z);

	logger.AddValue("L.FTS.Ankle_Fx", forceLeft.F.x);
	logger.AddValue("L.FTS.Ankle_Fy", forceLeft.F.y);
	logger.AddValue("L.FTS.Ankle_Fz", forceLeft.F.z);
	logger.AddValue("L.FTS.Ankle_Tx", forceLeft.T.x);
	logger.AddValue("L.FTS.Ankle_Ty", forceLeft.T.y);
	logger.AddValue("L.FTS.Ankle_Tz", forceLeft.T.z);

	// Расчет координаты ZMP
	double fz = forceLeft.F.z + forceRight.F.z;
	double ty = forceLeft.T.y + forceRight.T.y;
	double tx = forceLeft.T.x + forceRight.T.x;

	logger.AddValue("ZMP.X", ty / fz);
	logger.AddValue("ZMP.Y", tx / fz);
}
