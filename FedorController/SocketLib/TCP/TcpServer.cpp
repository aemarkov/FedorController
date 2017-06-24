#include "TcpServer.h"

using namespace SocketLib;

SocketLib::TcpServer::TcpServer()
{
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket == INVALID_SOCKET)
		throw SocketException(WSAGetLastError());
}

SocketLib::TcpServer::~TcpServer()
{
}

//Связывает с локальной конечной точкой
void TcpServer::Bind(sockaddr * addr, int length)
{
	if (addr == NULL)
		throw std::invalid_argument("addr is NULL");

	if (bind(_socket, addr, length) == SOCKET_ERROR)
		throw SocketException(WSAGetLastError());
}

//Связывает с локальной конечной точкой
void TcpServer::Bind(const char * address, int port)
{
	sockaddr_in addr;
	addr.sin_addr.S_un.S_addr = inet_addr(address);
	addr.sin_port = htons(port);
	addr.sin_family = AF_INET;

	Bind((sockaddr*)&addr, sizeof(addr));
}

//Связывает с локальной конечной точкой
void TcpServer::Bind(int port)
{
	sockaddr_in addr;
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	addr.sin_port = htons(port);
	addr.sin_family = AF_INET;

	Bind((sockaddr*)&addr, sizeof(addr));
}

//Начинает слушать с заданным числом подключений
void TcpServer::Listen(int maxConnectons)
{
	if (listen(_socket, maxConnectons) == SOCKET_ERROR)
		throw SocketException(WSAGetLastError());
}

//Начинает слушать с максимально возможным числом подключений
void SocketLib::TcpServer::Listen()
{
	Listen(SOMAXCONN);
}

//Принимает подключение
Socket & TcpServer::Accept()
{
	sockaddr addr;
	int length = sizeof(addr);
	SOCKET_TYPE socket = accept(_socket, &addr, &length);

	if(socket==INVALID_SOCKET)
		throw SocketException(WSAGetLastError());

	return * (new Socket(socket));
}

//Закрывает сокет
void TcpServer::Close()
{

	DWORD result;

#ifdef _WIN32
	result = closesocket(_socket);
#elif __linux__
	result = close(_socket);
#endif

	if (result == SOCKET_ERROR)
		throw SocketException(WSAGetLastError());

	_socket = INVALID_SOCKET;
}
