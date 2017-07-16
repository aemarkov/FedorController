#include "CommandResult.h"

using namespace FedorControl;

FedorControl::CommandResult::CommandResult()
{
	_buffer = nullptr;
	_bufferLength = 0;
}

CommandResult::CommandResult(uint8_t* buffer, uint32_t length)
{
	_buffer = buffer;
	_bufferLength = length;
}

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
