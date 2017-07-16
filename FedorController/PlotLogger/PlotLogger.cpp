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

	_line.clear();
	_separator = separator;
	_isFrame = false;
	_isHeader = false;

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

	for (const auto & title : titles)
		_line.push_back(LineItem(title));


	FinishHeader();
}

//Добавляет еще одну переменную в заголоовк
void PlotLogger::AddTitle(std::string title)
{
	if (!_isHeader)
		_line.push_back(LineItem(title));
	else
		throw logic_error("Header is already finished");
}

//Завершает формирование заголовка
void PlotLogger::FinishHeader()
{
	if (!_isHeader)
	{
		if (_line.size() == 0)
			throw invalid_argument("Titles count shouldn't be 0");

		//Итератор на последнем элементе
		auto last = --_line.end();

		for (auto it = _line.begin(); it != _line.end(); ++it)
		{
			_file << it->name;;

			//Не ставим разделитель после последнего столбца
			if (it != last)
				_file << _separator;
		}

		_file.flush();
		_isHeader = true;
	}
	else
		throw logic_error("Header is already finished");
}

//Начинает фрейм (строку) - блок данных с одним значением абсциссы
void PlotLogger::BeginFrame(double x)
{
	if (!_isHeader)
		throw logic_error("Setup header first");


	// Если фрейм уже начат, то завершить предыдущий
	if (_isFrame)
		EndFrame();

	auto & item = _line.begin();

	// Повторный вызов с той же самой абсциссой
	if (item->value == x && _isFrame)
		return;

	item->value = x;
	item->isSet = true;

	_isFrame = true;
}

//Добавляет значение в текущий фрейм
void PlotLogger::AddValue(std::string name, double value)
{
	//Ищем элемент с нужным именем
	int itemIndex = -1;
	for (size_t i = 0; i < _line.size(); i++)
	{
		if (_line[i].name == name)
		{
			itemIndex = i;
			break;
		}
	}


	if (itemIndex == -1)
		throw invalid_argument("\"" + name + "\" doesn't present in header");

	_line[itemIndex].value = value;
	_line[itemIndex].isSet = true;
}

//Добавляет значение в текущий фрейм
void PlotLogger::AddValue(std::pair<std::string, double> value)
{
	AddValue(value.first, value.second);
}

//Добавлен ли заголовок
bool PlotLogger::IsHeader()
{
	return _isHeader;
}

//Открыт ли файл
bool PlotLogger::IsOpen()
{
	return _file.is_open();
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
		if (it->isSet)
			_file << it->value;

		//Не ставим разделитель после последнего столбца
		if (it != last)
			_file << _separator;
	}

	_file.flush();
}




PlotLogger::LineItem::LineItem()
{
}

PlotLogger::LineItem::LineItem(std::string name)
{
	this->name = name;
	isSet = false;
	value = 0;
}
