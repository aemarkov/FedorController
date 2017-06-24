/*
Кросс-платформенная микро-библиотека для сокетов
Обертка над старндартными Windows- и Linux-сокетами.

Все представлено в двух файлах для удобства подключения
в проект.

Предоставляет классы:
 - Socket:
   Просто сокет, базовый класс
     - recv
	 - recvfrom
	 - send
	 - close

 - UdpSocket:
   Реализует обмен данными по UDP
    - Создание UDP сокета (конструктор)
	- bind
	- connect
	- recv
	- recvfrom
	- send
	- close

 - TcpServer:
   TCP сокет в режиме сервера.
   Не наследуется от Socket и имеет методы
   только для подключение клиентов, но не для обмена
     - Создание TCP сокета (конструктор)
	 - bind
	 - listen
	 - accept

 - TcpClient:
   TCP сокет в режиме клиента (подключение к серверу)
    - Создание TCP сокета (конструктор)
	- connect
	- recv
	- recvfrom
	- send
	- close


ТЕКУЩИЙ СТАТУС:
 - Windows-версия протестирована
 - Linux версия может даже не скомпилируется
 - Коды ошибок не приведены к единому формату

РЕАЛИЗАЦИЯ:
 Для избавления от копипасты я применил множественное наследование.
 I am sorry.
*/

#ifndef __SOCKETLIB_H__
#define __SOCKETLIB_H__

#ifdef _WIN32
#include <WinSock2.h>
#include <Windows.h>
#include <stdint.h>
#define SOCKET_TYPE SOCKET

#elif __linux__
#include <sys/socket>

#define SOCKET_TYPE int;
#define SOCKET_ERROR            (-1)
#define INVALID_SOCKET			(SOCKET)(~0)
#define DWORD					uint32_t
#endif

#ifdef __linux__
int WSAGetLastError()
{
	return errno;
}
#endif

#include <stdexcept>
#include "SocketException.h"

namespace SocketLib
{

	//////////////////////////////////////////////////////////////////////////////////////////////////

	//private классы сделать нельзя
	//Служебный namespace, не используйте эти классы
	namespace Helpers
	{
		/**
		Базовый для всех остальных.
		Реализует только метод закрывания
		*/
		class BaseSocket
		{
		public:

			BaseSocket();
			~BaseSocket();

			/**
			Закрывает сокет
			*/
			void Close();

		protected:

			SOCKET_TYPE _socket;
		};

		/**
		Реализует методы привязывания сокета
		к локальной конечной точке
		(для сервера)
		*/
		class BindableSocket : public virtual BaseSocket
		{
		public:
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


		/**
		Реализует методы подключения к серверу
		(для клиента)
		*/
		class ConnectableSocket : public virtual BaseSocket
		{
		public:
			/**
			Устанавливает соединение
			\param[in] addr Адрес и порт точки подключения
			*/
			void Connect(sockaddr* addr);

			/**
			Устанавливает соединение
			\param[in] address IP адрес
			\param[in] port Порт
			*/
			void Connect(const char* address, int port);
		};
	}	

	//////////////////////////////////////////////////////////////////////////////////////////////////

	/**
	Базовый класс сокета, реализующий только обмен данными,
	не конкретизируя, что это - клиент/сервер, TCP/UDP
	*/

	class Socket : public virtual Helpers::BaseSocket
	{
	public:

		/**
		Инициализирует WinSock
		*/
		static void Init();

		Socket();
		Socket(SOCKET_TYPE socket);

		/**
		Передает данные
		\param[in] buffer Передаваемые данные
		\param[in] length Размер передавемых данных в байтах
		\return Число реально переданных байт
		*/
		int Send(const uint8_t* buffer, int length, int flags = 0) const;

		/**
		Принимает данные
		\param[in] buffer Указаетль на буфер, куда будут записаны данные
		\param[in] length Размер буфера в байтах
		\param[in] flags флаги
		*/
		int Recv(uint8_t* buffer, int length, int flags = 0) const;

		/**
		Принимает данные
		\param[in] buffer Указаетль на буфер, куда будут записаны данные
		\param[in] length Размер буфера в байтах
		\param[in] flags флаги
		\param[out] from - указатель на структуру, куда будет записан адрес отправителя
		\param[out] fromLength - размер структуры адреса отправителя
		*/
		int RecvFrom(uint8_t* buffer, int length, int flags, sockaddr* from, int* fromLength);

	};

	//////////////////////////////////////////////////////////////////////////////////////////////////

	/**
	\brief Сокет для UDP протокола

	Реализует функционал,
	используемый UDP:
	- создание UDP сокета
	- привязка к локальной конечной точке
	- "подключение"
	*/
	class UdpSocket : public virtual Socket, public virtual Helpers::BindableSocket, public virtual Helpers::ConnectableSocket
	{
	public:
		UdpSocket();
	};


	//////////////////////////////////////////////////////////////////////////////////////////////////

	/**
	\brief Клиентский сокет для TCP протокола

	Реализует функционал, используемый клиентом TCP:
	- создание TCP сокета
	- подключение к серверу
	*/

	class TcpClient : public virtual Socket, public virtual Helpers::ConnectableSocket
	{
	public:
		TcpClient();
	};

	/**
	\brief Серверный сокет для TCP протокола

	Реализует функционал, используемый сервером TCP:
	 - Создание TCP сокета
	 - Привязка к локальной конечной точке
	 - Слушание
	 - Подключение клиента
	*/
	class TcpServer : public virtual Helpers::BindableSocket
	{
	public:

		TcpServer();

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
	};



}

#endif