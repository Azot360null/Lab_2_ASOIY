/*
* Автор: Смирнов Денис
* Создано: 18.03.2022
* Версия: 2
**/

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}


