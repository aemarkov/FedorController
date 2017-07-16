#ifndef __SENSORSGROUP_H__
#define __SENSORSGROUP_H__

#include <string>
#include "AbstractGroup.h"
#include "SensorsGroup.h"

namespace FedorControl 
{
	/**
		Набор интерфейсов для работы с датчиками
	*/
	class SensorsGroup : public AbstractGroup
	{
	public:

		SensorsGroup(SocketLib::TcpClient & socket, AbstractGroup* parent);

		void GetValues();
		void List();

	private:
		// Унаследовано через AbstractGroup
		virtual std::string MyPrefix() override;
	};

}

#endif