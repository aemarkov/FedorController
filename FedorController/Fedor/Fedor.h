#ifndef __FEDOR_H__
#define __FEDOR_H__

#include <SocketLib/SocketLib.h>
#include "RobotGroup.h"
#include "VersionGroup.h"

namespace FedorControl
{
	/**
		Реализует интерфейс управления роботом
	*/
	class Fedor
	{	
	
	public:

		Fedor();
		~Fedor();

		/**
			Подключается к роботу

			\param[in] ip IP-адрес робота
			\param[in] port Порт робота
		*/
		void Connect(const char* ip, int port);

		/**
			Отключается от робота
		*/
		void Disconnect();

		/**
			Группа интерфейсов для работы с роботом
		*/
		RobotGroup & Robot();

		/**
			Группа интерфейсов для работы с версиями
			ПО и протокола
		*/
		VersionGroup & Version();

	private:

		/*Возможность отстрелить ногу:
		Порядок инициализации определяется порядком объявления,
		поэтому сокет должен быть объявлен ПЕРЕД группами*/
		SocketLib::TcpClient _socket;

		RobotGroup _robot;
		VersionGroup _version;	

	};
}

#endif