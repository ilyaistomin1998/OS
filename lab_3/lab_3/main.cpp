#include "catalogoftourists.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    catalogOfTourists w;
    w.show();

    return a.exec();
}
