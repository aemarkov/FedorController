#include "SocketException.h"
#include <string>

using namespace SocketLib;

SocketException::SocketException()
{
}

SocketException::SocketException(uint32_t error) : exception(("Error code: " + std::to_string(error)).c_str())
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