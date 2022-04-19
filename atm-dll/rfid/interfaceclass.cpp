#include "interfaceclass.h"

interfaceclass::interfaceclass()
{
    pSerialCardReader = new SerialCardReader;

}

interfaceclass::~interfaceclass()
{
    delete pSerialCardReader;
    pSerialCardReader = nullptr;

}

QString interfaceclass::GetCardNumber()
{
   pSerialCardReader->SerialRead();
   CardNumber=pSerialCardReader->outTrimmed;

   return CardNumber;
}
