#ifndef __SOCKETLIB_H__
#define __SOCKETLIB_H__

#include "WindowsFix.h"
#include <stdexcept>
#include "SocketException.h"

namespace SocketLib
{

	/**
	\brief Кросс-платформенная микро-библиотека для сокетов

	Обертка над старндартными Windows- и Linux-сокетами.
	Базовый класс сокета, реализующий только обмен данными,
	не конкретизируя, что это - клиент/сервер, TCP/UDP
	*/

	class Socket
	{
	public:

		/**
		Инициализирует WinSock
		*/
		static void Init();

		Socket();
		Socket(SOCKET_TYPE socket);

		~Socket();


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

		/**
		Закрывает сокет
		*/
		void Close();

	protected:

		SOCKET_TYPE _socket;

	};

}

#endif