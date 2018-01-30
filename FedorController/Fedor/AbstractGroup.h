#ifndef __ABSTRACTGROUP_H__
#define __ABSTRACTGROUP_H__

#include <SocketLib/SocketLib.h>
#include "Data/CommandResult.h"
#include <string>
#include <vector>
#include <iostream>

namespace FedorControl
{

	/**
		\brief Базовый класс групп управления роботом

		Реализует функции отправки и получения команд роботу.

		Реализует механизм наследования префиксов групп:
		  получая в конструкторе указатель на родителя, при запросе
		  префикса, он запрашиввает префиксы всех родителей, разделяя
		  их через ":".

		  Цепочка префиксов строится только один раз, а затем
		  сохраняется в переменной для ускорения работы.
	*/
	class AbstractGroup
	{
	public:

		/**
			Создает группу
			\param[in] socket Ссылка на объект Socket, подключенный к роботу
			\param[in] parent Указатель на родительскую группу, используемый
							  для построения цепочки префиксов
		*/
		AbstractGroup(SocketLib::TcpClient & socket, AbstractGroup* parent = NULL);
		~AbstractGroup();

	protected:

		/**
			Возвращает цепочку префиксов для группы
		*/
		std::string Prefix();

		/**
			Отправляет команду роботу
		*/
		CommandResult SendCommand(std::string command) const;

		const FedorControl::CommandResult _SendCommand(std::string &command) const;

		/**
			Нарезает строку на подстроки по разделителю
			\param str Строка
			\param separator Разделитель, по которому будет разрезана строка
			\return Вектор подстрок
		*/
		static std::vector<std::string> Slice(std::string str, char separator);

		SocketLib::TcpClient & _socket;
		AbstractGroup* _parent;

	private:

		//Возвращает свой префикс
		virtual std::string MyPrefix() = 0;




		//Сохраненный префикс для оптимизации
		std::string _storedPrefix;

		//Буфер, используемый для получения ответов
		uint8_t* _recvBuffer;
		const uint32_t RECV_BUFFER_SIZE = 1024;
	};

}

#endif