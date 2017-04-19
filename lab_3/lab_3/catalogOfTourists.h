#ifndef CATALOGOFTOURISTS_H
#define CATALOGOFTOURISTS_H

#include <QtWidgets/QMainWindow>
#include "ui_catalogOfTourists.h"
#include "validator.h"
#include "tourists.h"
#include "CatalogDataBase.h"
#include <QFileDialog>

class catalogOfTourists : public QMainWindow
{
	Q_OBJECT

public:
	catalogOfTourists(QWidget *parent = 0);
	~catalogOfTourists();
	private slots:

	    /*
	     * Дезактивирует флаг виза, если выбрана страна Россия
		 * \param [in] index - индекс выбранного значения из перечня стран
	     */
		void changeRussia(int index);

		/*
		 * Добавляет 10 произвольных записей
		 */
		void writeRecord();

		/*
		 * Изменяет поля выделенной в браузере записи и вставляет ее на нужную позицию с учетом сортировки
		 */
		void saveRecord();

		/*
		 * Создает новую запись с полями из формы и помещает ее в массив записей и в браузер
		 */
		void createRecord();

		/*
		 * Удаляет выделенную запись
		 */
		void removeRecord();

		/*
		 * Выводит содержимое записей на поля формы
		 * \param [in] currentRow - индекс выделенной строки
		 * \param [in] currentColumn - индекс выделенного столбца
		 */
		void chooseRecord(int currentRow, int currentColumn);

		/*
		 * Открывает файл для чтения
		 */
		void openFile();

		/*
		 * Сохраняет базу данных в выбранный файл
		 */
		void saveFile();

		/*
		 * Сохраняет базу данных в выбранный файл как
		 */
		void saveFileAs();

		/*
		 * Создает новый файл для базы данных
		 */
		void createFile();

		/*
		 * Действие при закрытии окна приложения
		 */
		void closeEvent(QCloseEvent *event);

private:
	Ui::catalogOfTouristsClass ui;

	//записи баззы данных
	CatalogDataBase baseData;
	QString filename;
	
	//количество добавленных записей
	int count;

	/*
	 * Заполняет ячейки строки таблицы полями записи
	 * \param [in] index - индекс строки таблицы, которую нужно заполнить
	 * \param [in] other - экземпляр класса записей, поля которго нужно записать
	 */
	void setDataTable(int index, const List::S & other);

	/*
	 * Удаляет запись с заданным идентификатором
	 * \param [in] index - индекс записи(строки) в браузере, в которой находится идентификатор
	 */
	void deleteRecord(int index);

	/*
	 * Проверяет корректность введенных данных
	 * \return true, если поля место отдыха, стоимость и продолжительность заполены корректно, иначе false
	 */
	bool checkForCorrect() const;
};

#endif // CATALOGOFTOURISTS_H
