#include "TcpClient.h"

using namespace SocketLib;

TcpClient::TcpClient()
{
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket == INVALID_SOCKET)
		throw SocketException(WSAGetLastError());
}
