#include "AbstractGroup.h"

using namespace FedorControl;

AbstractGroup::AbstractGroup(SocketLib::TcpClient & socket, AbstractGroup * parent) : _socket(socket), _storedPrefix("")
{
	_parent = parent;
	_recvBuffer = new uint8_t[RECV_BUFFER_SIZE];
}

AbstractGroup::~AbstractGroup()
{
	if(_recvBuffer!=nullptr)
	{
		delete _recvBuffer;
		_recvBuffer = nullptr;
	}
}

//Возвращает цеочку префиксов
std::string AbstractGroup::Prefix()
{
	//Если префикс не сохранен, то получаем свой префикс
	//  и всех родителей
	if (_storedPrefix.length() == 0)
	{
		if (_parent == nullptr)
			_storedPrefix = MyPrefix() + ':';
		else
			_storedPrefix = _parent->Prefix() + MyPrefix() + ':';
	}

	return _storedPrefix;
}

//Проверяет ответ на правильность
bool IsResultFormatCorrect(CommandResult result)
{
	uint8_t* buffer = result.Buffer();
	uint32_t length = result.Length();

	return result.Code() == SUCCESS_WITH_RESULT &&
		length >= 3 &&
		buffer[length - 2] == '\r' &&
		buffer[length - 1] == '\n';
}

//Отправка команды роботу
CommandResult AbstractGroup::SendCommand(std::string command)
{
	command += "\r\n";

	try
	{
		return _SendCommand(command);
	}
	catch (SocketLib::SocketException & ex)
	{
		if (!_socket.Reconnect())
		{
			std::cout << "Error: " << ex.GetErrorCode() << "\n";
			return CommandResult();
		}

		return _SendCommand(command);
	}
	catch (std::string & ex)
	{
		std::cout << "Error: " << ex << "\n";
	}

	return CommandResult();

}

const FedorControl::CommandResult &FedorControl::AbstractGroup::_SendCommand(std::string &command)
{
	int sended = _socket.Send((uint8_t*)command.c_str(), command.length());

	int received = _socket.Recv(_recvBuffer, RECV_BUFFER_SIZE);

	auto result = CommandResult(_recvBuffer, received);

	if (result.Code() == SUCCESS)
		return result;

	if (result.Code() == SUCCESS_WITH_RESULT)
	{
		if (IsResultFormatCorrect(result))
			return result;

		//Статус успеха, но формат команды неверный
		//TODO: Обработка ошибок
		throw std::string("Result format is invalid");
	}

	//Статус ошибки
	//TODO: Обработка ошибок
	throw std::string("Command executed with error status");

	return result;

	return CommandResult();
}

//Нарезает строку на подстроки по разделителю
std::vector<std::string> AbstractGroup::Slice(std::string str, char separator)
{
	int pos = 0;
	int matchPos;
	std::vector<std::string> substrings;

	while ((matchPos = str.find(separator, pos)) != std::string::npos)
	{
		std::string substr = str.substr(pos, matchPos - pos);
		pos = matchPos + 1;
		substrings.push_back(substr);
	}

	//Если после последнего токена нет разделителя (A;B;) -> (A;B)
	//то надо остаток забрать, но не до конца строки, а до \r\n
	int end = str.find("\r\n");

	if (pos != end)
	{
		std::string substr = str.substr(pos, end - pos);
		substrings.push_back(substr);
	}

	return substrings;
}