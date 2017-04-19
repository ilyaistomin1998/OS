#include "catalogOfTourists.h"
#include <QTime>
#include <QString>
#include <QList>
#include <qtextcodec.h>
#include <QMessageBox>

catalogOfTourists::catalogOfTourists(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.countries, SIGNAL(currentIndexChanged(int)), this, SLOT(changeRussia(int)));
	connect(ui.createRecord, SIGNAL(clicked()), this, SLOT(createRecord()));
	connect(ui.saveRecord, SIGNAL(clicked()), this, SLOT(saveRecord()));
	connect(ui.writeRecord, SIGNAL(clicked()), this, SLOT(writeRecord()));
	connect(ui.removeRecord, SIGNAL(clicked()), this, SLOT(removeRecord()));
	connect(ui.browser, SIGNAL(currentCellChanged(int, int, int, int)), this, SLOT(chooseRecord(int, int)));
	connect(ui.openFile, SIGNAL(triggered()), this, SLOT(openFile()));
	connect(ui.SaveFile, SIGNAL(triggered()), this, SLOT(saveFile()));
	connect(ui.saveFileAs, SIGNAL(triggered()), this, SLOT(saveFileAs()));
	connect(ui.createFile, SIGNAL(triggered()), this, SLOT(createFile()));
	connect(ui.centralWidget, SIGNAL(closeEvent(QCloseEvent*)), this, SLOT(closeEvent(QCloseEvent*)));
	ui.browser->verticalHeader()->setHidden(true);
	ui.statusBar->setVisible(false);
	ui.mainToolBar->setVisible(false);
	Validator *val = new Validator(this);
	ui.placeComfort->setValidator(val);
	count = 0;
	ui.length->setValue(2);
	ui.price->setValue(3000);
}

catalogOfTourists::~catalogOfTourists()
{

}

void catalogOfTourists::changeRussia(int index)
{
	//если индекс выбранной страны равен 0
	if (!index)
	{
		//дезактивируем флаг виза
		ui.visa->setDisabled(true);

		//считаем, чтофлаг не выставлен
		ui.visa->setChecked(false);
	}
	else
		//иначе активизируем флаг
		ui.visa->setDisabled(false);
}

void catalogOfTourists::saveRecord()
{
	//если поля введены не корректно, то выходим
	if (!checkForCorrect() || ui.browser->currentRow()==-1) return;
	List rad;	//создаем экземпляр класса записи
	
	//сохраняем выбранную страну
	rad.countries = ui.countries->currentText();

	//сохраняем введеную продолжительность
	rad.length = ui.length->value();

	//сохраняем введеное место отдыха
	rad.model.placeOfComfort = rad.placeComfort = ui.placeComfort->text();

	//сохраняем введеную стоимость
	rad.model.price = rad.price = ui.price->value();

	//сохраняем выбранный тип комфорта
	rad.model.typeOfComfort = rad.typeComfort = ui.typeComfort->currentText();

	//сохраняем состояние флага
	rad.visa = ui.visa->isChecked();

	//сохраняем идентификатор текущей записи
	rad.model.id = rad.id = ui.browser->item(ui.browser->currentRow(), 1)->data(Qt::UserRole).toInt();

	//запоминаем номер выделенного столбца таблицы
	int col = ui.browser->currentColumn();

	//удаляем старую запись
	ui.browser->removeRow(ui.browser->currentRow());

	//находим место вставки новой записи и вставляем новую запись на эту позицию
	insertIndex = baseData.update(rad);

	//в таблицу добавляем новую строку
	ui.browser->insertRow(insertIndex);

	//заполняем поля таблицы
	setDataTable(insertIndex, rad.model);

	//выделяем в таблице добавленную запись
	ui.browser->setCurrentCell(insertIndex, col);
}

void catalogOfTourists::writeRecord()
{
	QTime midnight(0,0,0);	
	qsrand(midnight.secsTo(QTime::currentTime()));
	
	//контейнер для вариантов места отдыха
	QList<QString> list;

	//запоминаем индекс выденной строки
	int row = (ui.browser->currentRow() == -1) ? 0 : ui.browser->currentRow();

	//запоминаем индекс выделенного столбца
	int cul = (ui.browser->currentColumn() == -1) ? 0 : ui.browser->currentColumn();
	QTextCodec* c = QTextCodec::codecForLocale();

	//заполним контейнер возможными значениями
	list << c->toUnicode("Сочи") << c->toUnicode("Турция") << c->toUnicode("Туапсе") <<c->toUnicode("Дубай") << c->toUnicode("Афины");
	List rad;
	for (short i = 0; i < 10; i++)
	{
		//выбраем произвольное значение из перечня типа комфорта
		rad.model.typeOfComfort = rad.typeComfort = ui.typeComfort->itemText(qrand() % 8);

		//выбраем произвольное значение из перечня стран
		rad.countries = ui.countries->itemText(qrand() % 4);

		//сохраем произвольную продолжительность
		rad.length = qrand() % 31;

		//сохраняем произвольную стоимость
		rad.model.price = rad.price = qrand() % 56 + 5137;

		//сохраняем случайное состояние флага виза
		rad.visa = (qrand() % 2);

		//выбирам случайное значение из контейнера вариантов места отдыха
		rad.model.placeOfComfort = rad.placeComfort = list[qrand() % 5];

		//ищем позицию вставки новой в массиве записей и вставляем ее на эту позицию
		insertIndex = baseData.append(rad);

		//добавляем в таблицу новую строку
		ui.browser->insertRow(insertIndex);

		//заполняем поля добавленное строки
		setDataTable(insertIndex, rad.model);
	}
	
	//выделяем последнюю выделенную ячейку
	ui.browser->setCurrentCell(row, cul);
}

void catalogOfTourists::createRecord()
{
	//если поля записи заполнены неверно, то выходим
	if (!checkForCorrect()) return;

	//запоминаем индекс выделенного столбца таблицы
	int col = ui.browser->currentColumn();
	
	//создаем экземпляр класса записи
	List rad;

	//сохраняем выбранный тип комфорта
	rad.model.typeOfComfort = rad.typeComfort = ui.typeComfort->currentText();
	
	//сохраняем выбранную страну
	rad.countries = ui.countries->currentText();
	
	//сохраняем введеную продолжительность
	rad.length = ui.length->value();
	
	//сохраняем введеную стоимость
	rad.model.price = rad.price = ui.price->value();
	
	//сохраняем состояние флага виза
	rad.visa = ui.visa->isChecked();

	//сохраняем введеное место отдыха
	rad.model.placeOfComfort = rad.placeComfort = ui.placeComfort->text();

	//ищем позицию вставки новой записи в массив записей и вставляем ее туда
	insertIndex = baseData.append(rad);

	//добавляем строку в таблицу
	ui.browser->insertRow(insertIndex);

	//заполняем поля добавленной строки
	setDataTable(insertIndex, rad.model);

	//выделяем добавленную запись
	ui.browser->setCurrentCell(insertIndex, col);
}

void catalogOfTourists::removeRecord()
{
	//запоминаем индекс выделенной строки
	int currentIndex = ui.browser->currentRow();

	//сохраняем индекс выделл=енного столбца
	int cul = ui.browser->currentColumn();

	//удаляем выделенную запись
	deleteRecord(ui.browser->currentRow());

	//отключим кнопки создать и заполнить, если количество записей, 
	//добавляемых этими кнопками, превысит максимальный объем, выделяемый для 1000 записей 
	ui.createRecord->setEnabled((1000 - ui.browser->rowCount()) >= 1);
	ui.writeRecord->setEnabled((1000 - ui.browser->rowCount()) >= 10);

	//отключим кнопки удалить и сохранить, если в браузере не осталось записей
	ui.removeRecord->setEnabled(ui.browser->rowCount());
	ui.saveRecord->setEnabled(ui.browser->rowCount());
	
	//если в браузере не осталось записей
	if (!ui.browser->rowCount())
	{
		//создаем экземпляр класса записи со стандартными значениями
		List rad = List();

		//выберем стандартное значение из перечня стран
		ui.countries->setCurrentText(rad.countries);

		//выберем стандартное значение из перечня вида отдыха
		ui.typeComfort->setCurrentText(rad.typeComfort);

		//заполним поле продолжительность стандартным значением
		ui.length->setValue(rad.length);

		//заполним поле место отдыха стандартным значением
		ui.placeComfort->setText(rad.placeComfort);

		//заполним поле стоимость стандартным значением
		ui.price->setValue(rad.price);

		//установим флаг в стандартное значение
		ui.visa->setChecked(rad.visa);
	}
	else
	{
		//если индекс выделенной записи не является последним
		if ((currentIndex+1) != ui.browser->rowCount())
		{
			//и выходит за границу количества записей
			if (currentIndex == ui.browser->rowCount())
			{
				//уменьшить индекс выделенной строки
				currentIndex--;
			}
		}
		else
		{
			//если индекс выделенной строки не является нулевым
			if (currentIndex != 0)
			{
				//увеличить индекс выделенной строки
				currentIndex++;
			}
		}
		//выделяем ячейку в таблице
		ui.browser->setCurrentCell(currentIndex, cul);
	}
}

void catalogOfTourists::setDataTable(int index, const List::S & other)
{	
	//помещаем содержимое поля вид отдыха новой записи в соответствующую ячейку строки
	QTableWidgetItem *item = new QTableWidgetItem(other.typeOfComfort);
	ui.browser->setItem(index, 0, item);

	//помещаем содержимое поля место отдыха новой записи в соответствующую ячейку строки
	QTableWidgetItem *item1 = new QTableWidgetItem(other.placeOfComfort);
	ui.browser->setItem(index, 1, item1);

	//помещаем содержимое поля стоимость новой записи в соответствующую ячейку строки
	QTableWidgetItem *item2 = new QTableWidgetItem(QString::number(other.price));
	ui.browser->setItem(index, 2, item2);

	//присвоить новой записи в таблице уникальных идентификатор
	ui.browser->item(index, 1)->setData(Qt::UserRole, other.id);
}

void catalogOfTourists::chooseRecord(int currentRow, int currentColumn)
{
	//если ячейка не выделена, то выходим
	if (currentRow == -1)
	{
		//создаем экземпляр класса записи со стандартными значениями
		List rad = List();

		//выберем стандартное значение из перечня стран
		ui.countries->setCurrentText(rad.countries);

		//выберем стандартное значение из перечня вида отдыха
		ui.typeComfort->setCurrentText(rad.typeComfort);

		//заполним поле продолжительность стандартным значением
		ui.length->setValue(rad.length);

		//заполним поле место отдыха стандартным значением
		ui.placeComfort->setText(rad.placeComfort);

		//заполним поле стоимость стандартным значением
		ui.price->setValue(rad.price);

		//установим флаг в стандартное значение
		ui.visa->setChecked(rad.visa);
	}
	else
	{
		List rad;
		baseData.record(ui.browser->item(currentRow, 1)->data(Qt::UserRole).toInt(), rad);
		//заполнить поле страна значением соответствующего поля из записи,
		//индексом которой является выделенная строка
		ui.countries->setCurrentText(rad.countries);
			
		//заполнить поле вид отдыха значением соответствующего поля из записи,
		//индексом которой является выделенная строка
		ui.typeComfort->setCurrentText(rad.typeComfort);

		//заполнить поле место отдыха значением соответствующего поля из записи,
		//индексом которой является выделенная строка
		ui.placeComfort->setText(rad.placeComfort);
	
		//заполнить поле продолжительность значением соответствующего поля из записи,
		//индексом которой является выделенная строка
		ui.length->setValue(rad.length);

		//заполнить поле стоимость значением соответствующего поля из записи,
		//индексом которой является выделенная строка
		ui.price->setValue(rad.price);

		//присвоить флагу виза значение соответствующего поля из записи,
		//индексом которой является выделенная строка
		ui.visa->setChecked(rad.visa);

		//проверить на выбранность страны Россия
		changeRussia(ui.countries->currentIndex());

		//активируем кнопки удалить и сохранить
		ui.removeRecord->setDisabled(false);
		ui.saveRecord->setEnabled(true);
	}
}

void catalogOfTourists::deleteRecord(int index)
{	
	if(index == -1) return;
	baseData.remove(ui.browser->item(index, 1)->data(Qt::UserRole).toInt());
	//удалить из таюлицы выделенную строку
	ui.browser->removeRow(ui.browser->currentRow());	
}

bool catalogOfTourists::checkForCorrect() const
{
	QTextCodec* c = QTextCodec::codecForLocale();

	//если первая символ строки поля место отдыха является строчной буквой
	if (ui.placeComfort->text()[0].isLower())
	{
		QMessageBox::information(ui.centralWidget, c->toUnicode("Некорректная запись"), c->toUnicode("Неверно указано место отдыха"));
		return false;
	}

	//если значение поля продолжительность вне отрезка [2; 31]
	if (ui.length->value() < 2 || ui.length->value() > 31)
	{
		QMessageBox::information(ui.centralWidget, c->toUnicode("Некорректная запись"), c->toUnicode("Неверно указана длительность"));
		return false;
	}

	//если значение поля стоимость вне отрезка [3000; 100 000]
	if (ui.price->value() < 3000 || ui.price->value() > 100000)
	{
		QMessageBox::information(ui.centralWidget, c->toUnicode("Некорректная запись"), c->toUnicode("Неверно указана стоимость путевки"));
		return false;
	}
	return true;
}

void catalogOfTourists::openFile()
{
	QTextCodec *c = QTextCodec::codecForLocale();
	//если база данных была модифицирована
	if (baseData.isModified())
	{
		//спросить у пользователя о сохранении изменений в базе данных
		QMessageBox::StandardButton button = QMessageBox::question(this, c->toUnicode("Имеются изменения в БД"), c->toUnicode("Сохранить изменения?"),
			QMessageBox::Yes | QMessageBox::No);
		//если сохранить
		if (button == QMessageBox::Yes)
		{
			//сохраняет изменения
			saveFileAs();
		}
	}

	//получаем имя файла с базой данных
	filename = QFileDialog::getOpenFileName(this, c->toUnicode("Открыть файл"), c->toUnicode(""), c->toUnicode("catalogOfTourists files(*.cot)"));

	//удалить все стрые записи из браузера
	while(ui.browser->rowCount())
		ui.browser->removeRow(ui.browser->rowCount()-1);

	//загрузить баззу данныхиз файла
	baseData.load(filename);
	const QVector<List::S> brows = baseData.records();
	QVector<List::S>::const_iterator it;
	//заполняем браузер записями
	for (it = brows.begin(); it != brows.end(); it++)
	{
		ui.browser->insertRow(it-brows.begin());
		setDataTable(it-brows.begin(), *it);
	}
	ui.browser->setCurrentCell(-1, 0);
}

void catalogOfTourists::saveFile()
{
	//если имя файла не задано
	if (filename.isEmpty())
		//сохранить как
		saveFileAs();
	else
		//сохранить изменения в файл
		baseData.save(filename);
}

void catalogOfTourists::saveFileAs()
{
	QString name = filename;
	QTextCodec *c = QTextCodec::codecForLocale();
	//получаем имя файла
	filename = QFileDialog::getSaveFileName(this, c->toUnicode("Сохранить как"), c->toUnicode(""), c->toUnicode("catalogOfTourists files(*.cot)"));
	if (filename.isEmpty())
		filename = name;
	else
		baseData.save(filename);
}

void catalogOfTourists::createFile()
{
	//если база данных была модифицирована
	QTextCodec *c = QTextCodec::codecForLocale();
	if (baseData.isModified())
	{
		//предложить сохранить изменения
		QMessageBox::StandardButton but = QMessageBox::question(this, c->toUnicode("Имеются изменнения в базе данных"), 
			c->toUnicode("Желаете сохранить изменения?"), QMessageBox::Yes | QMessageBox::No);
		if (but == QMessageBox::Yes)
			saveFileAs();
	}
	filename = "";
	//очистить браузер записей
	while(ui.browser->rowCount())
		ui.browser->removeRow(0);
	//очистить базу банных
	baseData.clear();
	ui.browser->setCurrentCell(-1,0);
}

void catalogOfTourists::closeEvent(QCloseEvent *event)
{
	QTextCodec *c = QTextCodec::codecForLocale();
	//если база данных была модифицирована
	if (baseData.isModified())
	{
		//предложить сохранить изменения
		QMessageBox::StandardButtons but = QMessageBox::question(this, c->toUnicode("Выход"), c->toUnicode("Сохранить базу данных?"),
			QMessageBox::Yes | QMessageBox::No);
		if (but == QMessageBox::Yes)
			saveFileAs();
	}
}
