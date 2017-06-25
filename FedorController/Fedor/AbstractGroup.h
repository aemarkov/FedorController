#ifndef __ABSTRACTGROUP_H__
#define __ABSTRACTGROUP_H__

#include <SocketLib/SocketLib.h>
#include <string>
#include <iostream>

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
	AbstractGroup(SocketLib::Socket & socket, AbstractGroup* parent = NULL);

protected:

	/**
	Возвращает цепочку префиксов для группы
	*/
	std::string Prefix();

	/**
	Отправляет команду роботу

	TODO: Получение ответа от робота
	*/
	void SendCommand(std::string command);

	SocketLib::Socket & _socket;
	AbstractGroup* _parent;

private:

	/**
	Возвращает свой префикс
	*/
	virtual std::string MyPrefix() = 0;

	//Сохраненный префикс для оптимизации
	std::string _storedPrefix;
};

#endif