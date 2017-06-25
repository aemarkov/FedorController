#include "VersionGroup.h"

FedorControl::VersionGroup::VersionGroup(SocketLib::Socket & socket, AbstractGroup * parent) : AbstractGroup(socket, parent)
{
}

std::string FedorControl::VersionGroup::MyPrefix()
{
	return "version";
}
