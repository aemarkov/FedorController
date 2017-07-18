#include "SensorsGroup.h"

using namespace FedorControl;

const string SensorsGroup::R_FTS_Ankle = "R.FTS.Ankle";
const string SensorsGroup::L_FTS_Ankle = "L.FTS.Ankle";

SensorsGroup::SensorsGroup(SocketLib::TcpClient & socket, AbstractGroup * parent) : AbstractGroup(socket, parent)
{
}

//Возвращает данные с датчика инерциальной навигации
IMUData SensorsGroup::GetIMU()
{
	string cmd = Prefix() + IMU_SENSOR_NAME + ";";
	auto result = SendCommand(cmd);
	auto values = Slice((const char*)result.Data(), ';');

	IMUData data;
	data.Euler = glm::vec3(stod(values[0]), stod(values[1]), stod(values[2]));
	data.Acc = glm::vec3(stod(values[3]), stod(values[4]), stod(values[5]));
	data.Gyro = glm::vec3(stod(values[6]), stod(values[7]), stod(values[8]));
	data.Mag = glm::vec3(stod(values[9]), stod(values[10]), stod(values[11]));
	//data.Quaternion = glm::vec4(stod(values[12]), stod(values[13]), stod(values[14]), stod(values[15]));

	return data;
}

//Возвращает данные с силометрических датчиков
map<string, ForceData> FedorControl::SensorsGroup::GetForce(vector<string> names)
{
	string cmd = Prefix();

	for (auto & name : names)
	{
		cmd += name + ";";
	}
	
	auto result = SendCommand(cmd);
	vector<string> sensorsValues;

	if (names.size() > 1)
	{
		//Когда несколько сенсоров, группы данных от них разделены ":"
		sensorsValues = Slice((const char*)result.Data(), ':');
	}
	else
		sensorsValues = { (const char*)result.Data() };

	map<string, ForceData> sensorsData;

	for (int i = 0; i < sensorsValues.size(); i++)
	{
		auto values = Slice(sensorsValues[i], ';');
		ForceData data;
		data.F = glm::vec3(stod(values[0]), stod(values[1]), stod(values[2]));
		data.T = glm::vec3(stod(values[3]), stod(values[4]), stod(values[5]));

		sensorsData.insert(pair<string, ForceData>(names[i], data));
	}

	

	return sensorsData;
}

//Возвращает данные со всех сенсоров
SensorsData FedorControl::SensorsGroup::GetSensorsData()
{
	return SensorsData();
}

//Возвращает список сенсоров
vector<string> FedorControl::SensorsGroup::List()
{
	return vector<string>();
}

std::string SensorsGroup::MyPrefix()
{
	return "sensors";
}
