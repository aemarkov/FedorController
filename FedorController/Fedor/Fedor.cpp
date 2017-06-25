#include "Fedor.h"

using namespace FedorControl;

Fedor::Fedor() : _socket(), _robot(_socket, NULL),  _version(_socket, NULL)
{
}

void FedorControl::Fedor::Connect(const char * ip, int port)
{
	_socket.Connect(ip, port);
}

RobotGroup & FedorControl::Fedor::Robot()
{
	return _robot;
}

VersionGroup & FedorControl::Fedor::Version()
{
	return _version;
}
