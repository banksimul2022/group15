#include "asyncserialinterface.h"
#include "asyncserialengine.h"
#include <QObject>
#include <QSerialPort>
#include <QString>

AsyncSerialInterface::AsyncSerialInterface(QObject *parent) : QObject(parent)
{

    pSerialCardReader = new AsyncSerialEngine;
    connect(pSerialCardReader,SIGNAL(IDdone(QString)),this,SLOT(HandleSerialEngine(QString)));
}

AsyncSerialInterface::~AsyncSerialInterface()
{
    disconnect(pSerialCardReader,SIGNAL(IDdone(QString)),this,SLOT(HandleSerialEngine(QString)));
    delete pSerialCardReader;
    pSerialCardReader = nullptr;
}

void AsyncSerialInterface::StartReadingSerial()
{
    QSerialPort serialport;
    AsyncSerialEngine async(&serialport);
}

void AsyncSerialInterface::HandleSerialEngine(QString ID)
{
  emit CardID(ID);
}

