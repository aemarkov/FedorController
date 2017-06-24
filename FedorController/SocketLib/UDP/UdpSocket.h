#ifndef __UDPSOCKET_H__
#define __UDPSOCKET_H__

#include <stdexcept>
#include "../SocketLib.h"


namespace SocketLib
{
	/**
		\brief Сокет для UDP протокола

		Наследуется от базового Socket и реализует функционал,
		используемый UDP:
		- создание UDP сокета
		- привязка к локальной конечной точке
		- "подключение"
	*/
	class UdpSocket : public Socket
	{
	public:
		UdpSocket();

		/**
		Связывает сокет с заданной локальной конечной точкой
		\param[in] addr Структура, описывающая адрес
		\param[in] length Размер структуры addr
		*/
		void Bind(sockaddr* addr, int length);

		/**
		Связывает сокет с заданной локальной конечной точкой
		\param[in] address IP адрес слушания
		\param[in] port Порт слушания
		*/
		void Bind(const char* address, int port);

		/**
		Связывает сокет с заданной локальной конечной точкой с любым
		адресом (прослушиванием со всех интерфейсов)
		\param[in] port Порт слушания
		*/
		void Bind(int port);
	};
}

#endif