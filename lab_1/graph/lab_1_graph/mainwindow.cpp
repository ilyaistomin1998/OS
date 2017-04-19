#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->crypt, SIGNAL(clicked(bool)), this, SLOT(cryptFiles()));
    connect(ui->createFile, SIGNAL(clicked(bool)), this, SLOT(createFiles()));
    connect(ui->openFilesGedit, SIGNAL(clicked(bool)), this, SLOT(openFilesGedit()));
    connect(ui->removeFile, SIGNAL(clicked(bool)), this, SLOT(removeFiles()));
    connect(ui->uncrypt, SIGNAL(clicked(bool)), this, SLOT(uncryptFiles()));
    connect(ui->unzip, SIGNAL(clicked(bool)), this, SLOT(unzipFiles()));
    connect(ui->zip, SIGNAL(clicked(bool)), this, SLOT(zipFiles()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::cryptFiles()
{
    QStringList files = ui->fileNames->toPlainText().split(QRegularExpression(QString("\n")));
    for (int i = 0; i < files.size(); i++)
    {
        QStringList names = files[i].split(QRegularExpression(QString(" -> ")));
        if (!fork())
        {
            execl("lab_1", "lab_1", "/crpt", names[0].toStdString().c_str(), names[1].toStdString().c_str(), NULL);
        }
        else
        {
            wait(NULL);
        }
    }

}

void MainWindow::createFiles()
{
    QStringList files = ui->fileNames->toPlainText().split(QRegularExpression(QString("\n")));
    for (int i = 0; i < files.size(); i++)
        if (!fork())
        {
            execl("lab_1", "lab_1", "/new", files[i].toStdString().c_str(), NULL);
        }
        else
        {
            wait(NULL);
        }
}

void MainWindow::openFilesGedit()
{
    QStringList files = ui->fileNames->toPlainText().split(QRegularExpression(QString("\n")));
    for (int i = 0; i < files.size(); i++)
        if (!fork())
        {
            execl("lab_1", "lab_1", files[i].toStdString().c_str(), NULL);
        }
}

void MainWindow::removeFiles()
{
    QStringList files = ui->fileNames->toPlainText().split(QRegularExpression(QString("\n")));
    for (int i = 0; i < files.size(); i++)
        if (!fork())
        {
            execl("lab_1", "lab_1", "/del", files[i].toStdString().c_str(), NULL);
        }
        else
        {
            wait(NULL);
        }
}

void MainWindow::uncryptFiles()
{
    QStringList files = ui->fileNames->toPlainText().split(QRegularExpression(QString("\n")));
    for (int i = 0; i < files.size(); i++)
    {
        QStringList names = files[i].split(QRegularExpression(QString(" -> ")));
        if (!fork())
        {
            execl("lab_1", "lab_1", "/uncrpt", names[0].toStdString().c_str(), names[1].toStdString().c_str(), NULL);
        }
        else
        {
            wait(NULL);
        }
    }

}

void MainWindow::unzipFiles()
{
    QStringList files = ui->fileNames->toPlainText().split(QRegularExpression(QString("\n")));
    for (int i = 0; i < files.size(); i++)
    {
        if (!fork())
        {
            execl("lab_1", "lab_1", "/unzip", files[i].toStdString().c_str(), NULL);
        }
        else
        {
            wait(NULL);
        }
    }

}

void MainWindow::zipFiles()
{
    QStringList files = ui->fileNames->toPlainText().split(QRegularExpression(QString("\n")));
    for (int i = 0; i < files.size(); i++)
    {
        if (!fork())
        {
            execl("lab_1", "lab_1", "/zip", files[i].toStdString().c_str(), NULL);
        }
        else
        {
            wait(NULL);
        }
    }

}
