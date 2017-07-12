#ifndef __FEDORCONSOLE_H__
#define __FEDORCONSOLE_H__

#include <iostream>
#include <string>
#include <map>
#include <Fedor\Fedor.h>

namespace FedorControl
{
	using namespace std;

	void FedorConsole(Fedor fedor)
	{
		string drive;
		double angle;

		while (true)
		{
			cout << "> ";
			cin >> drive >> angle;
			if (drive == "")
				break;

			map<string, double> poses = {
				{ drive, angle }
			};

			fedor.Robot().Motors().Posset(poses);

		}
	}
}


#endif