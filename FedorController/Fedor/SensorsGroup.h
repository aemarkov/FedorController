#ifndef __SENSORSGROUP_H__
#define __SENSORSGROUP_H__

#include <string>
#include "AbstractGroup.h"
#include "SensorsGroup.h"

namespace FedorControl 
{

	class SensorsGroup : public AbstractGroup
	{
	public:

		SensorsGroup(SocketLib::Socket & socket, AbstractGroup* parent);

		void GetValues();
		void List();

	private:
		// Унаследовано через AbstractGroup
		virtual std::string MyPrefix() override;
	};

}

#endif