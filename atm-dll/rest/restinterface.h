#ifndef RESTINTERFACE_H
#define RESTINTERFACE_H

#include "REST_global.h"
#include "restreturndata.h"

#include <QObject>
#include <QSettings>

class REST_EXPORT RESTInterface : public QObject
{
    Q_OBJECT

public:
    static RESTInterface *createInstance(QSettings *settings);

    explicit RESTInterface(QObject *parent = nullptr);
    virtual ~RESTInterface() =0;
    virtual void login(QString cardNumber, QString pin)=0;
    virtual void getInfo() =0;
    virtual void logout() =0;
    virtual void changePin(QString pin, QString newPin) =0;
    virtual void latestTransaction(int count) =0;
    virtual void nextTransactions(int count) =0;
    virtual void prevTransactions(int count) =0;
    virtual void resetTransactionPageIndex() =0;
    virtual void withdraw(double sum, bool useCredit) =0;
    virtual void deposit(double sum) =0;
    virtual void showBalance() =0;
    virtual void transfer(QString accountNumber, double sum) =0;

private:




signals:
     void dataReturn(RestReturnData *data);
};

#endif // RESTINTERFACE_H
