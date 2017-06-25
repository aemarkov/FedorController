#include "AbstractGroup.h"

AbstractGroup::AbstractGroup(SocketLib::Socket & socket, AbstractGroup * parent) : _socket(socket), _storedPrefix("")
{
	_parent = parent;
}

//Возвращает цеочку префиксов
std::string AbstractGroup::Prefix()
{
	//Если префикс не сохранен, то получаем свой префикс
	//  и всех родителей
	if (_storedPrefix.length() == 0)
	{
		if (_parent == NULL)
			_storedPrefix = MyPrefix() + ':';
		else
			_storedPrefix = _parent->Prefix() + MyPrefix() + ':';
	}

	return _storedPrefix;
}

//Отправка команды роботу
void AbstractGroup::SendCommand(std::string command)
{
	command += "\r\n";
	char* buffer[1000];

	try
	{
		int sended = _socket.Send((uint8_t*)command.c_str(), command.length());
	}
	catch (SocketLib::SocketException & ex)
	{
		std::cout << "Error: " << ex.GetErrorCode() << "\n";
	}

	//int received = _socket.Recv((uint8_t*)buffer, 1000);

	//TODO: обработать результат
}
