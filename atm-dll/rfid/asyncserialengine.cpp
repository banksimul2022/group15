#include "asyncserialengine.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QString>
#include <QObject>
#include <QDebug>

AsyncSerialEngine::AsyncSerialEngine(QObject *parent) : QObject(parent)
{
    CardReader = new QSerialPort;
    SerialCardReaderGetCOM();
    CardReader->setPortName(CardReaderCOMport);
    CardReader->setBaudRate(QSerialPort::Baud9600);
    CardReader->setDataBits(QSerialPort::Data8);
    CardReader->setParity(QSerialPort::NoParity);
    CardReader->setStopBits(QSerialPort::OneStop);
    CardReader->setFlowControl(QSerialPort::NoFlowControl);
    CardReader->open(QIODevice::ReadOnly); //Starts serial port
    connect(CardReader,SIGNAL(readyRead()), this, SLOT(HandleRead()));
}

AsyncSerialEngine::~AsyncSerialEngine()
{
    disconnect(CardReader,SIGNAL(readyRead()), this, SLOT(HandleRead()));
    CardReader->close();
}

void AsyncSerialEngine::SerialCardReaderGetCOM()
{
    foreach(const QSerialPortInfo &qSPI,QSerialPortInfo::availablePorts()){
        if((qSPI.productIdentifier() == CardReaderProductID) && (qSPI.vendorIdentifier() == CardReaderVendorID)){
            CardReaderCOMport = qSPI.portName();
        }
    }
}

QString AsyncSerialEngine::SerialRead()
{
    outRAW.clear();  //Clears outRaw
    outTrimmed.clear(); //Clears outTrimmed
    //CardReader->waitForReadyRead(); //Waits for output, ??? theoretically could be replaced with "connect(CardReader, SIGNAL(readyRead()), ****, SLOT(SerialRead()));" in .exe ???
    byteOut= CardReader->readAll(); //Reads output
    outRAW += QString::fromStdString(byteOut.toStdString()); //Converts output to string
    rnTrim= outRAW.split("\r\n");
    outTrimmed = rnTrim[1]; //Takes string 1 with information
    outTrimmed.remove(0,1); //Removes first 4 symbols
    //outTrimmed.chop(3); //Removes last 3 symbols;
    qDebug() << outTrimmed << outRAW;

    return outTrimmed;
}

void AsyncSerialEngine::HandleRead()
{
    SerialRead();
    emit IDdone(outTrimmed);
}




