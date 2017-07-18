#pragma once

#include "IMUData.h"
#include "ForceData.h"
#include <map>
#include <string>

namespace FedorControl
{
	class SensorsData
	{
	public:
		IMUData IMU;
		std::map<std::string, ForceData> Forces;
	};
}