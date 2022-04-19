/*
* Автор: Смирнов Денис
* Создано: 18.03.2022
* Версия: 3
*
* Соавтор: Скворцов Даниил
* 19.04 - Добавил checkBox приостановления работы программы. Мелкие правки кода
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


