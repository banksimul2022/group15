#ifndef RESTENGINE_H
#define RESTENGINE_H

#include "restinterface.h"
#include "restreturnheader.h"
#include <QNetworkAccessManager>
#include <QUrl>
#include <QUrlQuery>


class RESTEngine : public RESTInterface
{
    Q_OBJECT
public:
    explicit RESTEngine(QString endpointstr, QObject *parent = nullptr);
    explicit RESTEngine(QUrl endpointurl, QObject *parent = nullptr);   
    void login(QString cardNumber, QString pin) override;
    void logout() override;
    void changePin(QString pin, QString newPin) override;
    void getInfo() override;
    void latestTransaction(int count) override;
    void nextTransactions(int count) override;
    void prevTransactions(int count) override;
    void resetTransactionPageIndex() override;
    void withdraw(double sum, bool useCredit)override;
    void deposit(double sum) override;
    void showBalance() override;
    void transfer(QString accountNumber, double sum) override;




private:
    const QUrl apiEndpoint;
    QNetworkAccessManager *Manager;
    QByteArray token;


    QNetworkRequest createRequest(QString path, QUrlQuery params, RestReturnData::ReturnType type, QString contentType=nullptr);
    QNetworkRequest createRequest(QString path, RestReturnData::ReturnType type, QString contentType=nullptr);
    QNetworkRequest createRequest(QUrl url, RestReturnData::ReturnType type, QString contentType);

    int nextOffset;
    int prevOffset;
    int lastError;
    QJsonObject lastData;


signals:



private slots:
    void replySlot(QNetworkReply *reply);



};

#endif // RESTENGINE_H
