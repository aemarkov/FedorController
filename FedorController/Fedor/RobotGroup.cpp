#include "RobotGroup.h"

using namespace FedorControl;

RobotGroup::RobotGroup(SocketLib::TcpClient & socket, AbstractGroup * parent) : AbstractGroup(socket, parent), _motors(socket, this), _sensors(socket, this)
{
}

std::string FedorControl::RobotGroup::MyPrefix()
{
	return "robot";
}

MotorsGroup & FedorControl::RobotGroup::Motors()
{
	return _motors;
}

SensorsGroup & FedorControl::RobotGroup::Sensors()
{
	return _sensors;
}


