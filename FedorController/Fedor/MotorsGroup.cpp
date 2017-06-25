#include "MotorsGroup.h"

FedorControl::MotorsGroup::MotorsGroup(SocketLib::Socket & socket, AbstractGroup * parent) : AbstractGroup(socket, parent)
{
}

std::string FedorControl::MotorsGroup::MyPrefix()
{
	return "motors";
}

//Устанавливает углы поворота моторов
void FedorControl::MotorsGroup::Posset(std::map<std::string, double> motors)
{
	std::string cmd = Prefix();

	for(auto it = motors.begin(); it!=motors.end(); ++it)
		cmd += it->first + ";";
	
	cmd += ":posset:";

	for (auto it = motors.begin(); it != motors.end(); ++it)
		cmd += std::to_string(it->second) + ";";

	SendCommand(cmd);
}

//Получить текущие углы выбранных приводов
std::map<std::string, double> FedorControl::MotorsGroup::Posget()
{
	return std::map<std::string, double>();
}

//Получить список моторов
std::vector<std::string> FedorControl::MotorsGroup::List()
{
	return std::vector<std::string>();
}
