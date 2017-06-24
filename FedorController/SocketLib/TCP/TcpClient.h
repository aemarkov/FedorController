#ifndef __TCPCLIENT_H__
#define __TCPCLIENT_H__

#include "../SocketLib.h"

namespace SocketLib
{

	/**

		\brief Клиентский сокет для TCP протокола

		Наследуется от базового Socket и реализует функционал,
		используемый клиентом TCP:
		 - создание TCP сокета
		 - подключение к серверу

	*/

	class TcpClient : public Socket
	{
	public:
		TcpClient();
	};

}

#endif