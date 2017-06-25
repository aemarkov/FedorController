
#include <map>
#include <iostream>
#include "Fedor\Fedor.h"
#include <stdint.h>

using namespace FedorControl;
using namespace std;
using namespace SocketLib;

int main()
{
	SocketLib::Socket::Init();

	Fedor fedor;
	fedor.Connect("127.0.0.1", 10099);

	string command;
	string value;

	while (true)
	{
		cout << "motor: ";
		cin >> command;

		if (command == "exit")
			break;

		cout << "value: ";
		cin >> value;

		std::map<std::string, double> wtf = {
			{command, stod(value)}
		};

		fedor.Robot().Motors().Posset(wtf);
	}

    return 0;
}

