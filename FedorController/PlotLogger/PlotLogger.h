#pragma once

#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <exception>

/**
	\brief Осуществляет логгирование значений в 
	       файл CSV для последующего построения графика

	Формат файла:
	x_xixle;v1_xixle;v2_xixle;v3_xixle
	x1;v1(x1);v2(x1);v3(x2)
	x2;v1(x2);v2(x2);v3(x2)
	...
	xn;v1(xn);v2(xn);v3(xn)


	Формат накладывает ограничения:
		1. Сначала надо установить заголовок для ВСЕХ значений
		2. Первый столбец - абсцисса для ВСЕХ значений
		3. Нельзя выводить данные в произвольном порядке, надо
		   сначала заполнить одну строку, потом другую и т.д.
*/
class PlotLogger
{
public:

	/**
		Создает новый экземпляр логгера
	*/
	PlotLogger();
	~PlotLogger();


	/**
		Создает файл для лога
		\param[in] filename Имя файла
		\param[in] Символ разделителя столбцов
	*/
	void Begin(std::string filename, char separator);

	/**
		Закрывает файл
	*/
	void Close();

	/**
		/brief Устанавливает заголовки столбцов

		В результате заголовок сразу завершен

		\param[in] titles Вектор заголовков
	*/
	void SetupHeaders(std::vector<std::string> titles);

	/**
		\brief Добавляет еще одну переменную в заголовок

		Можно сделать только пока заголовок не завершен функцией
		FinishHeader. Функция SetHeaders сразу завершает заголовок

		\param[in] title Имя переменной
	*/
	void AddTitle(std::string title);

	/**
		Завершает формирование заголовка
	*/
	void FinishHeader();

	/**
		/brief Начинает фрейм (строку) - блок данных с
		одним значением абсциссы

		Повторный вызов с тем же значением абсциссы ничего не делает.
		Вызов в с новым значением создает новую строку, и записывать
		в старую уже нельзя.

		\param[in] x Значение абсциссы
	*/
	void BeginFrame(double x);

	/**
		Добавляет значение в текущий фрейм
		\param[in] name Название значения, должно присутстовать 
		                в заголовке
		\param[in] value Значение
	*/
	void AddValue(std::string name, double value);

	/**
		Добавляет значение в текущий фрейм
		\param[in] value Название и значение. Название должно
		                 присутствовать в заголовке
	*/
	void AddValue(std::pair<std::string, double> value);


	/**
		Добавлен ли заголовок
	*/
	bool IsHeader();

	/**
		Открыт ли файл
	*/
	bool IsOpen();

private:

	struct LineItem
	{
		std::string name;
		double value;
		bool isSet;

		LineItem();
		LineItem(std::string name);
	};

	std::ofstream _file;										// Файл лога
	std::vector<LineItem> _line;								// Значения текущего фрейма
	char _separator;											// Разделитель столбцов
	bool _isFrame;												// Был ли начат фрейм
	bool _isHeader;												// Заголовок добавлен
	
	void EndFrame();
};
