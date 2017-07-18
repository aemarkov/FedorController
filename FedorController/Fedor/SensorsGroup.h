#ifndef __SENSORSGROUP_H__
#define __SENSORSGROUP_H__

#include <vector>
#include <string>
#include "AbstractGroup.h"
#include "SensorsGroup.h"
#include "Data/ForceData.h"
#include "Data/IMUData.h"
#include "Data/SensorsData.h"

namespace FedorControl 
{
	using namespace std;

	/**
		Набор интерфейсов для работы с датчиками
	*/
	class SensorsGroup : public AbstractGroup
	{
	public:

		SensorsGroup(SocketLib::TcpClient & socket, AbstractGroup* parent);

		/**
			Возвращает данные с датичка инерциальной
			навигации
		*/
		IMUData GetIMU();

		/**
			Возвращает данные с силометрических датчиков
			\param[in] name Список названий датчиков

			\return Словарь "название датчика - значение"
		*/
		map<string, ForceData>  GetForce(vector<string> names);
		
		/**
			Возвращает расстояние от ног до земли
		*/
		double GetDistance();
		
		/**
			Возвращает данные со всех сенсоров
		*/
		SensorsData GetSensorsData();

		/**
			Возвращает список сенсоров
			Не реализовано
		*/
		vector<string> List();

		//Константы имен сервисов
		static const string R_FTS_Ankle;
		static const string L_FTS_Ankle;

	private:
		
		//Префикс команды
		virtual string MyPrefix() override;

		//Имена датчиков
		const string IMU_SENSOR_NAME = "IMU";

		const vector<string> FORCE_SENSORS_NAMES = {
			R_FTS_Ankle, L_FTS_Ankle
		};
	};
	

}

#endif