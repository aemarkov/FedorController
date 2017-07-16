
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <stdint.h>
#include <fstream>

#define _USE_MATH_DEFINES
#include <math.h>

#include <Fedor\Fedor.h>
#include <DrivemagParser\DrivemagParser.h>
#include <FedorConsole.h>

using namespace FedorControl;
using namespace std;
using namespace SocketLib;

//#define foreach(iterator, collection) for(auto iterator = collection.begin(); iterator != collection.end(); ++iterator)

void PlayFrame(DrivemagParser::RobotPos pos);
Fedor* fedor;

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
	fedor->Connect("79.170.167.30", 58099);

	DrivemagParser::PlayDrivemag(filename, PlayFrame);

	fedor->Disconnect();
	delete fedor;
	return 0;
}

void PlayFrame(DrivemagParser::RobotPos pos)
{
	fedor->Robot().Motors().Posset(pos);
}