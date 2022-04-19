#ifndef INTERFACECLASS_H
#define INTERFACECLASS_H

#include "serialcardreader.h"
#include <QString>


class interfaceclass
{
public:
    interfaceclass();
    ~interfaceclass();
     QString GetCardNumber();
     QString CardNumber;




private:
    SerialCardReader * pSerialCardReader;



};

#endif // INTERFACECLASS_H
