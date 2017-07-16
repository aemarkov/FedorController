#include "MotorsGroup.h"

FedorControl::MotorsGroup::MotorsGroup(SocketLib::TcpClient & socket, AbstractGroup * parent) : AbstractGroup(socket, parent)
{
}

std::string FedorControl::MotorsGroup::MyPrefix()
{
	return "motors";
}

//Устанавливает углы поворота моторов
void FedorControl::MotorsGroup::Posset(std::map<std::string, double> motors)
{
	std::string cmd = Prefix();

	for (auto it = motors.begin(); it != motors.end(); ++it)
		cmd += it->first + ";";

	cmd += ":posset:";

	for (auto it = motors.begin(); it != motors.end(); ++it)
		cmd += std::to_string(it->second) + ";";

	SendCommand(cmd);
}


//Получить текущие углы выбранных приводов
std::map<std::string, double> FedorControl::MotorsGroup::Posget(std::vector<std::string> motors)
{
	std::string cmd = Prefix();

	for (auto it = motors.begin(); it != motors.end(); ++it)
		cmd += *it + ";";

	cmd += ":posget";

	auto result = SendCommand(cmd);
	if (result.Code() != SUCCESS_WITH_RESULT)
	{
		//Ошибка: на эту команду должен быть ответ SUCCESS_WITH_RESULT
		//TODO: обработка ошибок
	}

	//Нарезаем строку и собираем результат
	//result.Buffer()[result.Length() - 1] = 0;

	std::string str((char*)result.Data());
	std::map<std::string, double> angles;
	int pos = 0;
	int matchPos;
	size_t index = 0;

	while ((matchPos = str.find(';', pos)) != std::string::npos && index < motors.size())
	{
		std::string substr = str.substr(pos, matchPos - pos);
		pos = matchPos + 1;

		//Добавляем пару мотор-угол в мап
		try
		{
			angles.insert(std::make_pair(motors[index], std::stod(substr)));
			
		}
		catch (std::invalid_argument)
		{
			//Строка - не число
			//TODO: обработка ошибок
		}		

		index++;
	}


	if (index != motors.size() || pos < str.length())
	{
		//Количество полученных и запрошенных двигателей не совпадает
		//TODO: обработка ошибок
	}

	return angles;
}


//Получить текущие углы всех приводов
std::map<std::string, double> FedorControl::MotorsGroup::Posget()
{
	//Если нет списка моторов, то получаем его
	if (_motors.size() == 0)
		List();

	return Posget(_motors);
}


//Получить список моторов
std::vector<std::string> FedorControl::MotorsGroup::List()
{
	std::string cmd = Prefix() + "list";
	auto result = SendCommand(cmd);
	if (result.Code() != SUCCESS_WITH_RESULT)
	{
		//Ошибка: на эту команду должен быть ответ SUCCESS_WITH_RESULT
		//TODO: обработка ошибок
	}

	_motors = Slice((const char*)result.Data(), ';');

	//Если я верну этот объект, его нельзя будет изменить снаружи? 
	return _motors;
}
