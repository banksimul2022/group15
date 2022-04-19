//Don't forget to add "serialport" to your .pro file in "QT +=..." string
#include "serialcardreader.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QString>


SerialCardReader::SerialCardReader(QObject *parent) : QObject{parent}
{
    SerialCardReaderGetCOM();
    connect(&CardReader, &QSerialPort::readyRead, this, &SerialCardReader::ReadSerial);
    CardReader.setPortName(CardReaderCOMport);
    CardReader.setBaudRate(QSerialPort::Baud9600);
    CardReader.setDataBits(QSerialPort::Data8);
    CardReader.setParity(QSerialPort::NoParity);
    CardReader.setStopBits(QSerialPort::OneStop);
    CardReader.setFlowControl(QSerialPort::NoFlowControl);
    CardReader.open(QIODevice::ReadOnly); //Starts serial port

}

void SerialCardReader::SerialCardReaderGetCOM() //Must be called first in order to set serial port
{
    foreach(const QSerialPortInfo &qSPI,QSerialPortInfo::availablePorts()){
        if((qSPI.productIdentifier() == CardReaderProductID) && (qSPI.vendorIdentifier() == CardReaderVendorID)){
            CardReaderCOMport = qSPI.portName();
        }
    }
}

SerialCardReader::~SerialCardReader()
{
  CardReader.close();
}

void SerialCardReader::ReadSerial()
{
    QByteArray byteOut = CardReader.readAll(); //Reads output
    QString out = QString::fromStdString(byteOut.toStdString()); //Converts output to string
    out.remove(0, 4); //Removes first 4 symbols
    out.chop(3); //Removes last 3 symbols;
    emit cardRead(out);
}


