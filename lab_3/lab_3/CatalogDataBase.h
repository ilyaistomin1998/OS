#pragma once

#include "tourists.h"
#include <QVector>
#include <qmap.h>
#include <QFile>
#include <QDataStream>
#include <QTextStream>
class CatalogDataBase
{
public:
	CatalogDataBase(void);
	~CatalogDataBase(void);

	/*
	 * Возвращает количество хранимых записей
	 */
	int count() const;

	/*
	 * Присваивает уникальных идентификатор записи и добавляет в базу данных с учетом сортировки
	 * \param [in] record - запись, которую необходимо добавить
	 */
	int append (List & record);

	/*
	 * Удаляет запись с заданным идентификатором
	 */
	void remove (unsigned int id);

	/*
	 * Сохраняет внесенные изменения записи
	 * \param [in] record - запись, которую нужно обновить
	 */
	int update(const List & record);

	/*
	 * Возвращает запись с заданным идентификатором
	 * \param [in] id - уникальный идентификатор записи
	 * \param [out] record - возвращаемая запись
	 */
	void record (unsigned int id, List & record) const;

	/*
	 * Возвращает массив записей для браузера
	 */
	const QVector<List::S> records() const;

	/*
	 * Сохраняет базу данных в файл
	 * \param [in] filename - имя файла, в который нужно сохранить
	 */
	bool save(QString filename) const;

	/*
	 * Считывает базу данных из файла
	 * \param [in] filename - имя файла, с которого надо считать базу данных
	 */
	bool load(QString filename);

	/*
	 * Очищает баззу данных
	 */
	void clear();

	/*
	 * Проверяет была ли модифицирована база данных
	 */
	bool isModified() const;
	
private:
	//контейнер записей
	QMap <uint, List> rec;
	//контейнер идентификаторов
	QList <uint> ids;
	//максимальный на текущий момент идентификатор
	uint maxId;
	//флаг модифицированности базы данных
	mutable bool modified;
	//перегрузка потоков ввода/вывода
	friend QDataStream & operator<<(QDataStream & stream, const List & other);
	friend QDataStream & operator>>(QDataStream & stream, List & other);
};
