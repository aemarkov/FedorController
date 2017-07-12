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

		MotorsGroup(SocketLib::TcpClient & socket, AbstractGroup* parent);

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

		/**
		Получить текущие углы перечисленных приводов
		\param [in] motors Названия приводов, углы которых надо получить
		\return Словарь привод-угол для запрошенных углов
		*/
		std::map<std::string, double> Posget(std::vector<std::string> motors);

		/**
		Получить углы всех приводов
		\result Словарь привод-угол для всех углов
		*/
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

		//Список моторов
		//Используется для запросов всех положений моторов. Заполняется первый раз
		//при вызове получения списка илип положения всех моторов
		std::vector<std::string> _motors;
	};

}

#endif
