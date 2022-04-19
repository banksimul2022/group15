#ifndef SERIALCARDREADER_H
#define SERIALCARDREADER_H

#include "SerialCardReader_global.h"
#include <QSerialPort>
#include <QString>

class SERIALCARDREADER_EXPORT SerialCardReader : public QObject
{
    Q_OBJECT
public:
    explicit SerialCardReader(QObject *parent = nullptr); //Sets serial settings and starts serial port, remember calling SerialCardReaderGetCOM() ???first???
    void SerialCardReaderGetCOM(); //searches for device by comparing PorductID(CardReaderPorductID) and VendorID(CardReaderVendorID) and sets CardReaderCOMport to it if it did find one
    ~SerialCardReader();// Closes serial port
    QString CardReaderCOMport= "COM6"; //Default serial port for CardReader

signals:
    void cardRead(QString number);

private slots:
    void ReadSerial();



private:
    QSerialPort CardReader;
    short CardReaderProductID= 22; //Contains the product ID of CardReader
    short CardReaderVendorID= 5562; //Contains the VendorID of CardReader



};

#endif // SERIALCARDREADER_H
