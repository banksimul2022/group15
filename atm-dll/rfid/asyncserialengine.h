#ifndef ASYNCSERIALENGINE_H
#define ASYNCSERIALENGINE_H
#include <QSerialPort>
#include <QString>
#include <QObject>


class AsyncSerialEngine : public QObject
{
            Q_OBJECT
public:
    AsyncSerialEngine(QObject *parent =nullptr);
    ~AsyncSerialEngine();
    void SerialCardReaderGetCOM(); //Searches for device by comparing PorductID(CardReaderPorductID) and VendorID(CardReaderVendorID) and sets CardReaderCOMport to it if it did find one
    QString SerialRead(); //Reads serial COM
    QString CardReaderCOMport= "COM6"; //Default serial port for CardReader
    QString outTrimmed; //Trimmed output
    QSerialPort *CardReader;
signals:
    void IDdone(QString); //signals cardID
public slots:
    void HandleRead(); //Initiates SerialRead command

private:
    QByteArray byteOut; //QByteArray output, recives output before outRAW
    QString outRAW; //Serial RAW output
    QStringList rnTrim; //Divides outRAW into 3 strings, infomation is in string rnTrim[1]
    short CardReaderProductID= 22; //Contains productID of CardReader
    short CardReaderVendorID= 5562; //Contains vendorID of CardReader


};

#endif // ASYNCSERIALENGINE_H
