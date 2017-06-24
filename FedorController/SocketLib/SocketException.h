#ifndef __SOCKETEXCEPTION_H__
#define __SOCKETEXCEPTION_H__

#include <stdexcept>
#include <stdint.h>

namespace SocketLib
{

	/**
		\brief Исключение при работе с сокетами

		ВНИМАНИЕ: коды ошибок Windows и Linux могут
		различаться, пока универсальные коды не реализованы
	*/

	class SocketException : public std::exception
	{
	public:
		SocketException();
		SocketException(uint32_t error);
		SocketException(const char* message);

		int GetErrorCode();

	protected:
		int _errorCode;
	};

}

#endif