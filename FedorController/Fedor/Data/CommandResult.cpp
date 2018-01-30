#include "CommandResult.h"

#include <iostream>
#include <regex>

using namespace FedorControl;


CommandResult::CommandResult(uint8_t* buffer, uint32_t length)
{
	_buffer = buffer;
	_bufferLength = length;
}

/*CommandResult::~CommandResult()
{
	std::cout << "Destructor\n";
}

CommandResult::CommandResult(const CommandResult & other)
{
	std::cout << "Copy constructor\n";
	//Хз чем не угодил конструктор по-умолчанию, но в Release не работает адекватно
	_buffer = other._buffer;
	_bufferLength = other._bufferLength;
}


CommandResult & FedorControl::CommandResult::operator=(const CommandResult & other)
{
	std::cout << "=\n";
	_buffer = other._buffer;
	_bufferLength = other._bufferLength;

	return *this;
}*/

FedorResultCode CommandResult::Code()
{
	if (_buffer != nullptr && _bufferLength > 0)
		return (FedorResultCode)_buffer[0];  //Опасный код

	throw std::exception("Buffer is empty");
}

uint8_t * FedorControl::CommandResult::Buffer()
{
	return _buffer;
}

uint8_t * FedorControl::CommandResult::Data()
{
	if (_buffer != nullptr && _bufferLength > 1)
		return _buffer + 1;
	else
		return nullptr;
}

uint32_t FedorControl::CommandResult::Length()
{
	return _bufferLength;
}
