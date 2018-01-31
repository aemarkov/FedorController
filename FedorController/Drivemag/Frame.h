#pragma once

#include <string>
#include <map>

namespace FedorControl
{
	using namespace std;
	using Pose = std::map<std::string, double>;

	class Frame
	{
	public:
		double Time;
		Pose Pose;
	};
}