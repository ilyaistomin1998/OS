#include "CatalogDataBase.h"


CatalogDataBase::CatalogDataBase(void)
{
	maxId = 0;
	modified = false;
}

CatalogDataBase::~CatalogDataBase(void)
{
}

int CatalogDataBase::count() const
{
	//вернуть размер контейнера QMap
	return rec.size();
}

int CatalogDataBase::append(List & record)
{
	//иттератор по списку идентификаторов
	QList<uint>::iterator it;

	//присваиваем записи уникальный идентификатор
	record.id = record.model.id = ++maxId;
	uint idx = 0;

	for (it = ids.begin(); it != ids.end(); it++)
	{
		//если новая запись больше либо равна записи из контейнера
		if (rec[*it] <= record)
			//запоминем индекс вставки записи
			idx = it - ids.begin() + 1;
		else
			break;
	}
	//добавим новую запись
	rec.insert(record.id, record);
	//вместе с его идентификтором
	ids.insert(idx, record.id);
	//считаем, что модифицировали записи
	modified = true;
	return idx;
}

void CatalogDataBase::remove(unsigned int id)
{
	QList <uint>::iterator it;
	//удаляем запись
	rec.remove(id);
	for (it = ids.begin(); it != ids.end(); it++ )
	{
		//если идентификатор удаляемой записи равен индексу очередной записи
		if (*it == id)
		{
			//удаляем идентификатор
			ids.removeAt(it-ids.begin());
			break;
		}
	}
	modified = true;
}

int CatalogDataBase::update(const List & record)
{
	QList<uint>::iterator it;
	//удаляем запись
	remove(record.id);
	uint idx = 0;

	//ищем позицию вставки обновленной записи
	for (it = ids.begin(); it != ids.end(); it++)
	{
		if (rec[*it] <= record)
			idx = it - ids.begin() + 1;
		else
			break;
	}
	//добавляем запись в память
	rec.insert(record.id, record);
	ids.insert(idx, record.id);
	//считаем, что модифицировали базу данных
	modified = true;
	return idx;
}

void CatalogDataBase::record (unsigned int id, List & record) const
{
	record = rec[id];
}

const QVector<List::S> CatalogDataBase::records() const
{
	QVector<List::S> res;
	QList<uint>::const_iterator it;
	for (it = ids.begin(); it != ids.end(); it++)
	{
		res.push_back(rec[*it].model);
	}
	return res;
}

bool CatalogDataBase::save(QString filename) const
{
	//если имя файла существует
	if (!filename.isEmpty())
	{
		modified = false;
		QFile file(filename);
		//открываем файл только для записи
		if (file.open(QIODevice::WriteOnly))
		{
			//поток вывода
			QDataStream stream(&file);
			//вывод записи
			stream << rec;
			//выводим идентификаторы записей
			stream << ids;
			//закрываем файл
			file.close();
			return true;
		}
	}
	return false;
}

bool CatalogDataBase::load(QString filename)
{
	//если имя файла существует
	if (!filename.isEmpty())
	{
		QFile file(filename);
		//открываем файл только для чтения
		if (file.open(QIODevice::ReadOnly))
		{
			//поток для ввода/чтения
			QDataStream stream(&file);
			//очистим базу данных
			clear();
			//считываем записи
			stream >> rec;
			//считываем идентификаторы
			stream >> ids;
			//считаем, что модифицировали базу данных
			modified = true;
			QList<uint>::const_iterator it;
			//ищем максимальный идентификатор записи
			for(it = ids.begin(); it != ids.end(); it++)
			{
				if (maxId < rec[*it].id)
					maxId = rec[*it].id;
			}
			return true;
		}
	}
	return false;
}

void CatalogDataBase::clear()
{
	//очищаем перечень записей
	rec.clear();
	//очищаем перечень идентификаторов записей
	ids.clear();
	modified = true;
}

bool CatalogDataBase::isModified() const
{
	return modified;
}

QDataStream & operator<<(QDataStream & stream, const List & other)
{
	stream << other.countries << other.id << other.length << other.model.id << other.model.placeOfComfort << other.model.price <<
		other.model.typeOfComfort << other.placeComfort << other.price << other.typeComfort << other.visa;
	return stream;
}

QDataStream & operator>>(QDataStream & stream, List & other)
{
	stream >> other.countries >> other.id >> other.length >> other.model.id >> other.model.placeOfComfort >> other.model.price >>
		other.model.typeOfComfort >> other.placeComfort >> other.price >> other.typeComfort >> other.visa;
	return stream;
}
