#include "UdpSocket.h"

SocketLib::UdpSocket::UdpSocket()
{
	SOCKET wtf = INVALID_SOCKET;

	_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (_socket == INVALID_SOCKET)
		throw SocketException(WSAGetLastError());
}

void SocketLib::UdpSocket::Bind(sockaddr * addr, int length)
{
	if (addr == NULL)
		throw std::invalid_argument("addr is NULL");

	if (bind(_socket, addr, length) == SOCKET_ERROR)
		throw SocketException(WSAGetLastError());
}

void SocketLib::UdpSocket::Bind(const char * address, int port)
{
	sockaddr_in addr;
	addr.sin_addr.S_un.S_addr = inet_addr(address);
	addr.sin_port = htons(port);
	addr.sin_family = AF_INET;

	Bind((sockaddr*)&addr, sizeof(addr));
}

void SocketLib::UdpSocket::Bind(int port)
{
	sockaddr_in addr;
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	addr.sin_port = htons(port);
	addr.sin_family = AF_INET;

	Bind((sockaddr*)&addr, sizeof(addr));
}
