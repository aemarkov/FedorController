#ifndef __ROBOTGROUP_H__
#define __ROBOTGROUP_H_

#include <string>
#include "AbstractGroup.h"
#include "MotorsGroup.h"
#include "SensorsGroup.h"

namespace FedorControl
{

	class RobotGroup : public AbstractGroup
	{
	public:

		RobotGroup(SocketLib::Socket & socket, AbstractGroup* parent);

		MotorsGroup & Motors();
		SensorsGroup & Sensors();


	private:
		MotorsGroup _motors;
	    SensorsGroup _sensors;


		// Унаследовано через AbstractGroup
		virtual std::string MyPrefix() override;

	};

}

#endif
