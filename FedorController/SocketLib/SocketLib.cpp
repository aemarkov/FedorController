#include "SocketLib.h"

using namespace SocketLib;

//Инициализирует WinSock
void SocketLib::Socket::Init()
{
#ifdef _WIN32
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(1, 1), &wsaData);

	if (result != 0)
		throw SocketException(WSAGetLastError());

#endif
}

SocketLib::Socket::Socket()
{
	_socket = INVALID_SOCKET;
}

Socket::Socket(SOCKET_TYPE socket)
{
	_socket = socket;
}

Socket::~Socket()
{
	Close();
}

//Устанавливает соединение
void SocketLib::Socket::Connect(sockaddr* addr)
{
	if (connect(_socket, addr, sizeof(*addr)) == SOCKET_ERROR)
		throw SocketException(WSAGetLastError());

}

//Устанавливает соединение
void SocketLib::Socket::Connect(const char * address, int port)
{
	sockaddr_in addr;
	addr.sin_addr.S_un.S_addr = inet_addr(address);
	addr.sin_port = htons(port);
	addr.sin_family = AF_INET;

	Connect((sockaddr*)&addr);
}

//Отправляет данные
int Socket::Send(const uint8_t * buffer, int length, int flags) const
{
	if (buffer == NULL)
		throw std::invalid_argument("buffer is NULL");

	int result = send(_socket, (char*)buffer, length, flags);
	if(result == SOCKET_ERROR)
		throw SocketException(WSAGetLastError());
}

//Принимает данные
int Socket::Recv(uint8_t * buffer, int length, int flags) const
{
	if (buffer == NULL)
		throw std::invalid_argument("buffer is NULL");

	int result = recv(_socket, (char*)buffer, length, flags);
	if (result == SOCKET_ERROR)
		throw SocketException(WSAGetLastError());

	return result;
}

//Принимает данные и адрес отправителя
int Socket::RecvFrom(uint8_t * buffer, int length, int flags, sockaddr * from, int * fromLength)
{
	if(buffer == NULL)
		throw std::invalid_argument("buffer is NULL");

	//Если не указать рамзер перед передачей в функцию, будет ошибка
	*fromLength = sizeof(*from);

	int result = recvfrom(_socket, (char*)buffer, length, flags, from, fromLength);
	if (result == SOCKET_ERROR)
		throw SocketException(WSAGetLastError());

	return result;
}

//Закрывает сокет
void Socket::Close()
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
