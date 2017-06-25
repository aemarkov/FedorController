#ifndef __FEDOR_H__
#define __FEDOR_H__

#include <SocketLib/SocketLib.h>
#include "RobotGroup.h"
#include "VersionGroup.h"

namespace FedorControl
{
	class Fedor
	{	
	
	public:

		Fedor();

		/**
		Подключается к роботу
		*/
		void Connect(const char* ip, int port);


		RobotGroup & Robot();
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