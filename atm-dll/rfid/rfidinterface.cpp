#include "rfidinterface.h"

RFIDInterface::RFIDInterface(QObject *parent) : QObject{parent}
{
    pSerialCardReader = new SerialCardReader;
    connect(pSerialCardReader, &SerialCardReader::cardRead, this, &RFIDInterface::cardRead);
}

RFIDInterface::~RFIDInterface()
{
    delete pSerialCardReader;
    pSerialCardReader = nullptr;

}
