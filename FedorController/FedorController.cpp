
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include "Fedor\Fedor.h"
#include <stdint.h>
#include <fstream>

#define _USE_MATH_DEFINES
#include <math.h>

#include <FedorConsole.h>

using namespace FedorControl;
using namespace std;
using namespace SocketLib;

//#define foreach(iterator, collection) for(auto iterator = collection.begin(); iterator != collection.end(); ++iterator)



int main(int argc, char** argv)
{
	/*if (argc != 2)
	{
		cout << "Usage:\n";
		cout << "FedorController <drivemag.txt>\n";
		return -1;
	}

	char* filename = argv[1];*/


	

	Socket::Init();
	Fedor fedor;
	fedor.Connect("127.0.0.1", 10099);

	FedorConsole(fedor);

	return 0;
}

