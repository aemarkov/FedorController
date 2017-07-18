#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace FedorControl
{
	/**
		Данные с датчика инерциальной
		навигации
	*/
	class IMUData
	{
	public:
		//Углы (градусы)
		glm::vec3 Euler;

		//Данные с акксерометра (g)
		glm::vec3 Acc;

		//Данные с гироскопа (градус/с)
		glm::vec3 Gyro;

		//Данные с магнетометра (Gauss)
		glm::vec3 Mag;

		//???
		glm::vec4 Quaternion;
	};
}