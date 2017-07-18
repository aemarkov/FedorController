#pragma once

#include <glm/vec3.hpp>

/**
	Данные с силометрического датчика
*/

namespace FedorControl
{
	class ForceData
	{
	public:
		glm::vec3 F;
		glm::vec3 T;
	};
}