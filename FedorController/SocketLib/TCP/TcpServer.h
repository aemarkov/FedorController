#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__

#include "../WindowsFix.h"
#include "../SocketLib.h"

namespace SocketLib
{
	class TcpServer
	{
	public:

		TcpServer();
		~TcpServer();

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

		/**
		Начинает слушать подключения
		\param[in] maxConnections максимальное количество подключений
		*/
		void Listen(int maxConnections);

		/**
		Начинает слушать подключения c 
		максимальным числом подключений
		*/
		void Listen();

		/**
		Ожидает подкючения и возвращает новый Socket,
		который связан с подключившимся клиентом.
		Эта функция блокирующая.

		ВНИМАНИЕ: Происходит создание нового объекта Socket в 
		куче, не забудьте освободить память.

		\return Сокет, связанный с клиентом
		*/
		Socket& Accept();


		/**
		Закрывает сокет
		*/
		void Close();

	private:
		SOCKET_TYPE _socket;
	};
}

#endif