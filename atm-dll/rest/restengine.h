#ifndef RESTENGINE_H
#define RESTENGINE_H

#include "restinterface.h"
#include "restreturnheader.h"
#include <QNetworkAccessManager>
#include <QUrl>


class RESTEngine : public RESTInterface
{
    Q_OBJECT
public:
    explicit RESTEngine(QString endpointstr, QObject *parent = nullptr);
    explicit RESTEngine(QUrl endpointurl, QObject *parent = nullptr);
    ~RESTEngine();
    void login(QString cardNumber, QString pin) override;
    void logout() override;
    void getInfo() override;


private:
    const QUrl apiEndpoint;
    QNetworkAccessManager *Manager;
    QByteArray token;

    QNetworkRequest createRequest(QString path, RestReturnData::ReturnType type, QString contentType=nullptr );




signals:



private slots:
    void replySlot(QNetworkReply *reply);



};

#endif // RESTENGINE_H
