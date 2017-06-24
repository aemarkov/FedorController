#include "SocketException.h"

using namespace SocketLib;

SocketException::SocketException()
{
}

SocketException::SocketException(uint32_t error)
{
	_errorCode = error;
}

SocketLib::SocketException::SocketException(const char * message) : exception(message)
{
}

int SocketLib::SocketException::GetErrorCode()
{
	return _errorCode;
}
