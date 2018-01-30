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
	_ip = ip;
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

void Fedor::ResetScene()
{
	SocketLib::UdpSocket socket;
	socket.Connect(_ip.c_str(), 10098);

	std::string command = "scene:reset\r\n";
	int a = socket.Send((uint8_t*)command.c_str(), command.length());

	socket.Close();
}
