#ifndef __MOTORCOMMAND_H__
#define __MOTORCOMMAND_H__

#include <string>

namespace FedorControl
{

	/**
	Название мотора и значение, связанное с ним
	*/
	struct MotorCommand
	{
		std::string MotorName;
		double Value;
	};

}

#endif
