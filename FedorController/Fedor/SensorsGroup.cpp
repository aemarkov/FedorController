#include "SensorsGroup.h"

FedorControl::SensorsGroup::SensorsGroup(SocketLib::TcpClient & socket, AbstractGroup * parent) : AbstractGroup(socket, parent)
{
}

std::string FedorControl::SensorsGroup::MyPrefix()
{
	return "sensors";
}
