#pragma once

#include <string>
#include <map>

namespace FedorControl
{
	using namespace std;

	class Frame
	{
	public:
		double Time;
		map<string, double> Pose;
	};
}