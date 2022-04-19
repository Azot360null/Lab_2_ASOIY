/*
* Автор: Смирнов Денис
* Создано: 18.03.2022
* Версия: 3
*
* Соавтор: Скворцов Даниил
* 19.04 - Добавил checkBox приостановления работы программы. Мелкие правки кода
**/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QByteArray>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void Read();
    void Write();
    void TestVoid();

    void on_signalsBlocker_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;
    QSerialPort *Arduino;
    QByteArray Data;
    QString Buffer;
    QTimer *Timer;
    bool tryFirst;
    bool access;
    bool blockedTimer;

};
#endif // MAINWINDOW_H
