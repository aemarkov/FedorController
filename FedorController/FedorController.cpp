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
#include <CLI11.hpp>
#include <filesystem>


using namespace FedorControl;
using namespace std;
using namespace SocketLib;

void PlayDrivemags(vector<vector<Frame>> drivemags, double poseTransitionTime);
void LogStay(long milliseconds, Pose& stayingPose);
void PlayFrame(chrono::duration<float> t, map<string, double> & poses);
void PlayFrame_ToStart(chrono::duration<float> t, map<string, double> & poses);
void CreateLoggerHeader(Pose& pose);
void LogIMU();
void LogForces();
string GetLogFilename(string drivemagFilename);

FedorClock fedorClock;
DrivemagPlayer player(true, 5, 20, fedorClock);
Fedor* fedor;
PlotLogger logger;

bool noLog = false;


int main(int argc, char** argv)
{
	string ip = "127.0.0.1";
	int port = 10099;
	vector<string> drivemags_filenames;
	string log_name;
	bool noResetScene = false;
	double poseTransitionTime = 2.0;
	bool isDegrees = false;

	CLI::App app{ "Robot F.E.D.O.R. controll software" };
	app.add_option("-d,--drivemag", drivemags_filenames, "List of DRIVEMAG filenames")->required()->check(CLI::ExistingFile);
	app.add_option("-i,--ip", ip, "F.E.D.O.R. IP address");
	app.add_option("-p,--port", port, "F.E.D.O.R. port");
	app.add_option("-l,--log", log_name, "Log filename");
	app.add_option("-t,--transition", poseTransitionTime, "Pose to pose transitionTime");
	app.add_flag("--no-log", noLog, "Disable logging");
	app.add_flag("--no-reset", noResetScene, "Disable scene reset before playing");
	app.add_flag("--deg", isDegrees, "Disable scene reset before playing");

	CLI11_PARSE(app, argc, argv);

	Socket::Init();
	fedor = new Fedor();

	try
	{
		fedor->Connect(ip.c_str(), port);
	}
	catch(exception ex)
	{
		cout << "Can't connect to F.E.D.O.R:\n";
		cout << ex.what() << "\n";
		return -1;
	}
	
	vector<vector<Frame>> drivemags;
	for (string filename : drivemags_filenames)
		drivemags.push_back(DrivemagParser::LoadDrivemag(filename, isDegrees));

	if (!noLog)
	{		
		if (log_name.empty())
			log_name = GetLogFilename(drivemags_filenames[0]);

		logger.Begin(log_name, ' ');
		CreateLoggerHeader(drivemags[0][0].Pose);
	}	

	if (!noResetScene)
	{
		fedor->ResetScene();
		Sleep(500);
	}	

	PlayDrivemags(drivemags, poseTransitionTime);

	fedor->Disconnect();
	delete fedor;
	return 0;
}

void PlayDrivemags(vector<vector<Frame>> drivemags, double poseTransitionTime)
{
	cout << "Moving to start position...\n";
	player.ToStart(fedor->Robot().Motors().Posget(), drivemags[0][0].Pose, poseTransitionTime, PlayFrame);

	for (int i = 0; i<drivemags.size() - 1; i++)
	{
		cout << "Playing DRIVEMAG " << i << "...\n";
		player.PlayDrivemag(drivemags[i], PlayFrame);
		cout << "Moving to next DRIVEMAG...\n";
		player.ToStart(fedor->Robot().Motors().Posget(), drivemags[i + 1][0].Pose, poseTransitionTime, PlayFrame);
	}

	cout << "Playing DRIVEMAG " << drivemags.size() - 1 << "...\n";
	player.PlayDrivemag(drivemags[drivemags.size() - 1], PlayFrame);
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

// По имени файла генерирует имя лога
string GetLogFilename(string drivemagFilename)
{
	std::experimental::filesystem::path drivemag(drivemagFilename);
	auto logFilename = drivemag.parent_path() / experimental::filesystem::path("log_" + drivemag.filename().string());
	return logFilename.string();
}