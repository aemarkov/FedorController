#ifndef __ROBOTGROUP_H__
#define __ROBOTGROUP_H_

#include <string>
#include "AbstractGroup.h"
#include "MotorsGroup.h"
#include "SensorsGroup.h"

namespace FedorControl
{
	/**
		Набор интерфейсов для работы с роботом
	*/
	class RobotGroup : public AbstractGroup
	{
	public:

		RobotGroup(SocketLib::TcpClient & socket, AbstractGroup* parent);

		/**
			Набор интерфейсов для работы с моторами
		*/
		MotorsGroup & Motors();

		/**
			Набор интерфейсов для работы с датчиками
		*/
		SensorsGroup & Sensors();


	private:
		MotorsGroup _motors;
	    SensorsGroup _sensors;


		// Унаследовано через AbstractGroup
		virtual std::string MyPrefix() override;

	};

}

#endif
