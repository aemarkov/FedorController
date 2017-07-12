#include "VersionGroup.h"

FedorControl::VersionGroup::VersionGroup(SocketLib::TcpClient & socket, AbstractGroup * parent) : AbstractGroup(socket, parent)
{
}

std::string FedorControl::VersionGroup::MyPrefix()
{
	return "version";
}
