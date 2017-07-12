#ifndef __VERSIONGROUP_H__
#define __VERSIONGROUP_H__

#include <string>
#include "AbstractGroup.h"

namespace FedorControl 
{
	class VersionGroup : AbstractGroup
	{
	public:

		VersionGroup(SocketLib::TcpClient & socket, AbstractGroup* parent);

		void Shell();
		void Protocol();

	private:
		// Унаследовано через AbstractGroup
		virtual std::string MyPrefix() override;
	};
}

#endif