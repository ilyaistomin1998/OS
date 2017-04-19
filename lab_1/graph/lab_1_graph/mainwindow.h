#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>
#include <QRegularExpression>
#include <unistd.h>
#include <sys/wait.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:
    void openFilesGedit();
    void createFiles();
    void cryptFiles();
    void uncryptFiles();
    void zipFiles();
    void unzipFiles();
    void removeFiles();
};

#endif // MAINWINDOW_H
