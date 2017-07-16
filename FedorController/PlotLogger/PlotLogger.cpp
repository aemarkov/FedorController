#include "PlotLogger.h"

using namespace std;

PlotLogger::PlotLogger()
{
}

PlotLogger::~PlotLogger()
{
	Close();
}

//Создает файл для лога
void PlotLogger::Begin(std::string filename, char separator)
{

	_file.open(filename, fstream::out);
	if (!_file.is_open())
		throw std::runtime_error("Can't open file " + filename);

	_separator = separator;
	_isFrame = false;

}

//Закрывает файл
void PlotLogger::Close()
{
	if (_file.is_open())
	{
		EndFrame();
		_file.close();
	}
}

//Устанавливает заголовки столбцов
void PlotLogger::SetupHeaders(std::vector<std::string> titles)
{
	if (!_file.is_open())
		throw logic_error("Open file first");

	if (titles.size() == 0)
		throw invalid_argument("Titles count shouldn't be 0");

	for (const auto  & title: titles)
		_line.insert(pair<string, pair<double, bool>>(title, pair<double, bool>(0, false)));

	//Запись заголовков в файл

	//Итератор на последнем элементе
	auto last = --_line.end();

	for (auto it = _line.begin(); it != _line.end(); ++it)
	{
		_file << it->first;;

		//Не ставим разделитель после последнего столбца
		if (it != last)
			_file << _separator;
	}

	_file.flush();
}

//Начинает фрейм (строку) - блок данных с одним значением абсциссы
void PlotLogger::BeginFrame(double x)
{
	if (_line.size() == 0)
		throw logic_error("Setup header first");


	// Если фрейм уже начат, то завершить предыдущий
	if (_isFrame)
		EndFrame();

	auto & value = _line.begin()->second;

	// Повторный вызов с той же самой абсциссой
	if (value.first == x && _isFrame)
		return;

	value.first = x;
	value.second = true;

	_isFrame = true;
}

//Добавляет значение в текущий фрейм
void PlotLogger::AddValue(std::string name, double value)
{
	auto & it = _line.find(name);
	if (it == _line.end())
		throw invalid_argument("\"" + name + "\" doesn't present in header");

	it->second.first = value;
	it->second.second = true;
}

//Добавляет значение в текущий фрейм
void PlotLogger::AddValue(std::pair<std::string, double> value)
{
}

// Завершает фрейм
void PlotLogger::EndFrame()
{
	if (!_isFrame)
		return;

	//Итератор на последнем элементе
	auto last = --_line.end();

	_file << "\n";

	for (auto it = _line.begin(); it != _line.end(); ++it)
	{
		if (it->second.second)
			_file << it->second.first;

		//Не ставим разделитель после последнего столбца
		if(it != last)
			_file << _separator;
	}

	_file.flush();
}
