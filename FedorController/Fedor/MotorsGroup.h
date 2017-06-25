#ifndef __MOTORSGROUP_H__
#define __MOTORSGROUP_H__

#include <string>
#include <map>
#include <vector>
#include "AbstractGroup.h"

namespace FedorControl
{

	class MotorsGroup : public AbstractGroup
	{
	public:

		MotorsGroup(SocketLib::Socket & socket, AbstractGroup* parent);

		//Устанавливает углы поворота моторов
		void Posset(std::map<std::string, double> motors);

		//Устанавливает скорости моторов
		//Не поддерживается
		void Velset(std::map<std::string, double> motors);

		//Устанавливает момент моторов
		//Не поддерживается
		void Torqset(std::map<std::string, double> motors);

		//Перемещение приводов в заданное положение за заданное время
		//Не поддерживается
		void Go();

		//Получить текущие углы выбранных приводов
		std::map<std::string, double> Posget();

		//Отключить привод и зашелкнуть тормозную муфты
		//Не поддерживается
		void MotorsBreak();

		//Отключить привод и расщелкнуть тормозную муфту
		//Не поддерживается
		void MotorsRelax();

		//Получает режим управления, напряжение, ток, температуру
		//Не поддерживается
		void Status();

		//Получить список моторов
		std::vector<std::string> List();

	private:
		// Унаследовано через AbstractGroup
		virtual std::string MyPrefix() override;
	};

}

#endif
