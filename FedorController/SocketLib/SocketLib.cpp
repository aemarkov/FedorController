#include "SocketLib.h"

using namespace SocketLib;

//////////////////////////////////////////////////////////////////////////////////////////////////
//                                  Служебные базовые классы                                   ///
//////////////////////////////////////////////////////////////////////////////////////////////////


//------------------------------------- BaseSocket ----------------------------------------------

Helpers::BaseSocket::BaseSocket()
{
	_socket = INVALID_SOCKET;
}

Helpers::BaseSocket::~BaseSocket()
{
	Close();
}

//Закрывает сокет
void Helpers::BaseSocket::Close()
{
	DWORD result;

	if (_socket == INVALID_SOCKET)
		return;

#ifdef _WIN32
	result = closesocket(_socket);
#elif __linux__
	result = close(_socket);
#endif

	//Если сокет не был подключен, попытка его
	//закрыть вернет SOCKET_ERROR

	//if (result == SOCKET_ERROR)
	//	throw SocketException(WSAGetLastError());

	_socket = INVALID_SOCKET;
}

//------------------------------------- Bindable Socket------------------------------------------

void Helpers::BindableSocket::Bind(sockaddr * addr, int length)
{
	if (addr == NULL)
		throw std::invalid_argument("addr is NULL");

	if (bind(_socket, addr, length) == SOCKET_ERROR)
		throw SocketException(WSAGetLastError());
}

void Helpers::BindableSocket::Bind(const char * address, int port)
{
	sockaddr_in addr;
	addr.sin_addr.S_un.S_addr = inet_addr(address);
	addr.sin_port = htons(port);
	addr.sin_family = AF_INET;

	Bind((sockaddr*)&addr, sizeof(addr));
}

void Helpers::BindableSocket::Bind(int port)
{
	sockaddr_in addr;
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	addr.sin_port = htons(port);
	addr.sin_family = AF_INET;

	Bind((sockaddr*)&addr, sizeof(addr));
}

//------------------------------------- Connectable Socket---------------------------------------

//Устанавливает соединение
void Helpers::ConnectableSocket::Connect(sockaddr* addr)
{
	if (connect(_socket, addr, sizeof(*addr)) == SOCKET_ERROR)
		throw SocketException(WSAGetLastError());

}

//Устанавливает соединение
void Helpers::ConnectableSocket::Connect(const char * address, int port)
{
	sockaddr_in addr;
	addr.sin_addr.S_un.S_addr = inet_addr(address);
	addr.sin_port = htons(port);
	addr.sin_family = AF_INET;

	Connect((sockaddr*)&addr);
}


//////////////////////////////////////////////////////////////////////////////////////////////////
//                                  Socket                                                     ///
//////////////////////////////////////////////////////////////////////////////////////////////////

//Инициализирует WinSock
void Socket::Init()
{
#ifdef _WIN32
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(1, 1), &wsaData);

	if (result != 0)
		throw SocketException(WSAGetLastError());

#endif
}

Socket::Socket()
{
	_socket = INVALID_SOCKET;
}

Socket::Socket(SOCKET_TYPE socket)
{
	_socket = socket;
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


//////////////////////////////////////////////////////////////////////////////////////////////////
//                                  UdpSocket                                                  ///
//////////////////////////////////////////////////////////////////////////////////////////////////


UdpSocket::UdpSocket()
{
	SOCKET wtf = INVALID_SOCKET;

	_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (_socket == INVALID_SOCKET)
		throw SocketException(WSAGetLastError());
}



//////////////////////////////////////////////////////////////////////////////////////////////////
//                                  TcpClient                                                  ///
//////////////////////////////////////////////////////////////////////////////////////////////////


TcpClient::TcpClient()
{
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket == INVALID_SOCKET)
		throw SocketException(WSAGetLastError());
}


//////////////////////////////////////////////////////////////////////////////////////////////////
//                                  TcpServer                                                  ///
//////////////////////////////////////////////////////////////////////////////////////////////////


TcpServer::TcpServer()
{
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket == INVALID_SOCKET)
		throw SocketException(WSAGetLastError());
}


//Начинает слушать с заданным числом подключений
void TcpServer::Listen(int maxConnectons)
{
	if (listen(_socket, maxConnectons) == SOCKET_ERROR)
		throw SocketException(WSAGetLastError());
}

//Начинает слушать с максимально возможным числом подключений
void TcpServer::Listen()
{
	Listen(SOMAXCONN);
}

//Принимает подключение
Socket & TcpServer::Accept()
{
	sockaddr addr;
	int length = sizeof(addr);
	SOCKET_TYPE socket = accept(_socket, &addr, &length);

	if (socket == INVALID_SOCKET)
		throw SocketException(WSAGetLastError());

	return *(new Socket(socket));
}
