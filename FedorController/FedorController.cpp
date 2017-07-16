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

void PlayFrame(double t, double sleepTime, map<string, double> & poses);

Fedor* fedor;
PlotLogger logger;


chrono::time_point<chrono::steady_clock> t0;

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

	t0 = chrono::steady_clock::now();
	DrivemagParser::PlayDrivemag(filename, PlayFrame);

	fedor->Disconnect();
	delete fedor;
	return 0;
}

void PlayFrame(double t, double sleepTime, map<string, double> & poses)
{
	if (!logger.IsHeader())
	{
		logger.AddTitle("Time");

		for (auto & pos : poses)
			logger.AddTitle(pos.first);

		auto motors = fedor->Robot().Motors().List();
		for (auto & motor : motors)
			logger.AddTitle(motor + "_real");
		

		logger.FinishHeader();
	}

	chrono::duration<double> t_ = chrono::steady_clock::now() - t0;

	logger.BeginFrame(t);
	for (auto & pos : poses)
		logger.AddValue(pos);
	
	fedor->Robot().Motors().Posset(poses);

	Sleep(sleepTime);

	auto realPoses = fedor->Robot().Motors().Posget();
	for (auto & pose : realPoses)
		logger.AddValue(pose.first + "_real", pose.second);
}