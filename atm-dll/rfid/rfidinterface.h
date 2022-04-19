#ifndef RFIDINTERFACE_H
#define RFIDINTERFACE_H

#include "serialcardreader.h"
#include <QString>


class RFIDInterface : public QObject
{
    Q_OBJECT
public:
    explicit RFIDInterface(QObject *parent = nullptr);
    ~RFIDInterface();

signals:
    void cardRead(QString number);


private:
    SerialCardReader * pSerialCardReader;



};

#endif // RFIDINTERFACE_H
