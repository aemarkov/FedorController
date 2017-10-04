#ifndef __COMANDRESULT_H__
#define __COMANDRESULT_H__

#include <stdint.h>
#include <exception>

namespace FedorControl
{
	/**
	Возможные коды результата после выполнения команды
	*/
	enum FedorResultCode
	{
		UNKNOW_ERROR = 0xFF,                                // Неизвестная команда или синтаксическая ошибка
		DEVICE_UNAVALABLE = 0xFE,                           // Оборудование или устройство не доступно
		UNKNOWN_IDENTIFIER = 0xFD,                          // Неизвестный идентификатор или устройство с данным идентификатором отсутствует
		INVALID_PARAMETER = 0xFC,                           // Не корректное значение входного параметра
		SUCCESS = 0xF0,                                     // Команда выполнена
		SUCCESS_WITH_RESULT = 0xF1,                         // Команда выполнена с возвратом результата
		COMMAND_OUTDATED = 0xF2,                            // Команда устарела, рекомендуется использовать обновленный вариант
		COMMAND_UNSUPPORTED = 0xF3,                         // Оборудование или устройство не поддерживает команду
		COMMAND_UNSUPPORTED_IN_CURRENT_VERSION = 0xF4       // Текущая версия программного комплекса не поддерживает команду
	};

	/**
	\brief Результат выполнения команды

	Обертка вокруг буфера результата, позволяюшая выделить из него
	код статуса и сами данные
	*/
	class CommandResult
	{

	public:
		
		CommandResult(uint8_t* buffer, uint32_t length);
		//CommandResult(const CommandResult & other);
		//~CommandResult();
		//CommandResult& operator=(const CommandResult& other);

		FedorResultCode Code();
		uint8_t* Buffer();
		uint8_t* Data();
		uint32_t Length();

	private:
		CommandResult();
		uint8_t* _buffer;
		uint32_t _bufferLength;

	};
}

#endif
