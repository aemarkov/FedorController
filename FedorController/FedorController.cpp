#include <WinSock2.h>
#include <Windows.h>


#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <stdint.h>
#include <chrono>

#include <Fedor\Fedor.h>
#include <DrivemagParser\DrivemagParser.h>
#include <FedorConsole.h>
#include <PlotLogger\PlotLogger.h>


using namespace FedorControl;
using namespace std;
using namespace SocketLib;

void PlayFrame(double t, map<string, double> & poses);

Fedor* fedor;
PlotLogger logger;


int main(int argc, char** argv)
{
	if (argc != 2)
	{
		cout << "Usage:\n";
		cout << "FedorController <drivemag.txt>\n";
		return -1;
	}

	char* filename = argv[1];


	Socket::Init();
	fedor = new Fedor();
	//fedor->Connect("79.170.167.30", 58099);
	fedor->Connect("127.0.0.1", 10099);


	logger.Begin("log.txt", ' ');

	DrivemagParser::PlayDrivemag(filename, PlayFrame);

	fedor->Disconnect();
	delete fedor;
	return 0;
}

void PlayFrame(double t, map<string, double> & poses)
{
	if (!logger.IsHeader())
	{
		logger.AddTitle("Time");

		for (auto & pos : poses)
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


		logger.FinishHeader();
	}


	fedor->Robot().Motors().Posset(poses);
	auto delayStart = chrono::steady_clock::now();
	chrono::duration<double> currentDelay;

	logger.BeginFrame(t);

	for (auto & pos : poses)
		logger.AddValue(pos);

	auto realPoses = fedor->Robot().Motors().Posget();

	for (auto & pose : realPoses)
		logger.AddValue(pose.first + "_real", pose.second);

	auto imu = fedor->Robot().Sensors().GetIMU();
	logger.AddValue("Yaw", imu.Euler.x);
	logger.AddValue("Pitch", imu.Euler.y);
	logger.AddValue("Roll", imu.Euler.z);

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

	currentDelay = chrono::steady_clock::now() - delayStart;


}