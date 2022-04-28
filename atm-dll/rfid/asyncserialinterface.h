//!!!Remember to add QT += serialport to your .pro file!!!
#ifndef ASYNCSERIALINTERFACE_H
#define ASYNCSERIALINTERFACE_H

#include "AsyncSerialInterface_global.h"
#include "asyncserialengine.h"
#include <QSerialPort>
#include <QObject>
#include <QString>

class ASYNCSERIALINTERFACE_EXPORT AsyncSerialInterface : public QObject
{
        Q_OBJECT
public:
    AsyncSerialInterface(QObject *parent =nullptr);
    ~AsyncSerialInterface();
    void StartReadingSerial(); //Starts reading serial
    QString CardNumber;
signals:
    void CardID(QString); //Sends card id to exe

public slots:
    void HandleSerialEngine(QString); //Waits for signal from AsyncSerialEngine
private:
     AsyncSerialEngine * pSerialCardReader;

};

#endif // ASYNCSERIALINTERFACE_H
