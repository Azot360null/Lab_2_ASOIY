/*
* Автор: Смирнов Денис
* Создано: 18.03.2022
* Версия: 3
*
* Соавтор: Скворцов Даниил
* 19.04 - Добавил checkBox приостановления работы программы. Мелкие правки кода
**/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPortInfo>
#include <QDebug>
#include <QMessageBox>
#include <QTimer>
#include <dos.h>
#include <unistd.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->temperatureTextBox->setText("0");
    ui->pressureTextBox->setText("0");
    blockedTimer = false;

    //Определяет порт, к которму подключено ардуино.

    Arduino = new QSerialPort(this);
    Buffer = "";
    tryFirst = true;
    access = false;

    bool arduino_is_available = false;
    QString arduino_uno_port_name;

    //  Для каждого доступного последовательного порта
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        //  проверяет, имеет ли последовательный порт идентификатор продукта и идентификатор поставщика
        if(serialPortInfo.hasProductIdentifier() && serialPortInfo.hasVendorIdentifier())
        {
            arduino_is_available = true;
            arduino_uno_port_name = serialPortInfo.portName();
        }
    }


    //Открытие и настройка порта Arduino, если он доступен.
    if(arduino_is_available)
    {
        QMessageBox::information(this, "Serial port info", "Имя порта: " + arduino_uno_port_name);
        qDebug() << "Найден порт Arduino...\n";

        //TestSpeed();

        Arduino->setPortName(arduino_uno_port_name);
        Arduino->open(QSerialPort::ReadWrite);
        Arduino->setBaudRate(QSerialPort::Baud9600);
        Arduino->setDataBits(QSerialPort::Data8);
        Arduino->setFlowControl(QSerialPort::NoFlowControl);
        Arduino->setParity(QSerialPort::NoParity);
        Arduino->setStopBits(QSerialPort::OneStop);
        Arduino->setDataTerminalReady(true);

        QObject::connect(Arduino, SIGNAL(readyRead()), this, SLOT(Read()));

        Timer = new QTimer(this);
        QObject::connect(Timer, SIGNAL(timeout()), this, SLOT(Write()));
        Timer->start(1000);
    }
    else
    {
        qDebug() << "Не удалось найти правильный порт для Arduino.\n";
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Read()
{
    if(tryFirst)
    {
        Data = Arduino->readAll();
        if(Data == "d")
        {
            qDebug() << "Test complited successful!";
            qDebug() << Data;
        }
        else
        {
            qDebug() << "The port settings are incorrect!";
            qDebug() << "Incorrect baud rate!";
            qDebug() << Data;
        }
        tryFirst = false;
        Buffer = ""; //Строка буфера сбрасывается
        return;
    }
    access = true;
    // Прочитать все данные из серийного номера, но не гарантировано всё, что нужно
    Data = Arduino->readAll();
    // Добавить новую информацию в буфер
    Buffer += QString(Data);
    // Проверяет конец буфера. Если конечный символ равен '\n', это означает, что данные полные
    if(Buffer.back() == '\n')
    {// Проверяется начало отправки
            QStringList buffer_split = Buffer.split(":"); // Разделяет данные на температуру и давление (формат: "ттт:ддд")

            ui->temperatureTextBox->setText(buffer_split[0]);
            ui->pressureTextBox->setText(buffer_split[1].split("\n")[0]);

            Buffer = ""; // Строка буфера сбрасывается
    }
}

void MainWindow::Write()
{
    Arduino->write("0"); // Отправляет запрос
}

void MainWindow::TestVoid()
{
    if (!access)
    {
        qDebug() << "The port settings are incorrect!";
        qDebug() << "Incorrect baud rate!";
        access = true;
    }
}

/* Скворцов: основной алгоритм для реализации работы checkBox'a */
void MainWindow::on_signalsBlocker_stateChanged(int arg1)
{
    if(blockedTimer == true)
    {
        Timer->blockSignals(false);
        blockedTimer = false;
    } else
    {
        Timer->blockSignals(true);
        blockedTimer = true;
    }
}

