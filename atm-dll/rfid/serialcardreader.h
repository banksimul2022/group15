#ifndef SERIALCARDREADER_H
#define SERIALCARDREADER_H

#include "SerialCardReader_global.h"
#include <QSerialPort>
#include <QString>

class SERIALCARDREADER_EXPORT SerialCardReader
{
public:
    SerialCardReader(); //Sets serial settings and starts serial port, remember calling SerialCardReaderGetCOM() ???first???
    void SerialCardReaderGetCOM(); //searches for device by comparing PorductID(CardReaderPorductID) and VendorID(CardReaderVendorID) and sets CardReaderCOMport to it if it did find one
    ~SerialCardReader();// Closes serial port
    QString SerialRead(); //Reads serial COM
    QString CardReaderCOMport= "COM6"; //Default serial port for CardReader
    QString outTrimmed; //Trimmed output






private:
    QSerialPort CardReader;
    QByteArray byteOut; //QByteArray output, recives output before outRAW
    QString outRAW; //Serial RAW output
    QStringList rnTrim = outRAW.split("\r\n"); //Divides outRAW into 3 strings, infomation is in string rnTrim[1]
    short CardReaderProductID= 22; //Contains the product ID of CardReader
    short CardReaderVendorID= 5562; //Contains the VendorID of CardReader



};

#endif // SERIALCARDREADER_H
