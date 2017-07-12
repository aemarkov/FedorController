#include "Fedor.h"

using namespace FedorControl;

Fedor::Fedor() : _socket(), _robot(_socket, NULL),  _version(_socket, NULL)
{
}

FedorControl::Fedor::~Fedor()
{
	Disconnect();
}

void FedorControl::Fedor::Connect(const char * ip, int port)
{
	_socket.Connect(ip, port);
}

void FedorControl::Fedor::Disconnect()
{
	_socket.Close();
}

RobotGroup & FedorControl::Fedor::Robot()
{
	return _robot;
}

VersionGroup & FedorControl::Fedor::Version()
{
	return _version;
}
