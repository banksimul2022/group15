//Don't forget to add "serialport" to your .pro file in "QT +=..." string
#include "serialcardreader.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QString>


SerialCardReader::SerialCardReader()
{
    SerialCardReaderGetCOM();
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

QString SerialCardReader::SerialRead()
{
  outRAW.clear();  //Clears outRaw
  outTrimmed.clear(); //Clears outTrimmed
  CardReader.waitForReadyRead(); //Waits for output, ??? theoretically could be replaced with "connect(CardReader, SIGNAL(readyRead()), ****, SLOT(SerialRead()));" in .exe ???
  byteOut= CardReader.readAll(); //Reads output
  outRAW += QString::fromStdString(outRAW.toStdString()); //Converts output to string
  outTrimmed = rnTrim[1]; //Takes string 1 with information
  outTrimmed.remove(0,4); //Removes first 4 symbols
  outTrimmed.chop(3); //Removes last 3 symbols;

  return outTrimmed;
}


