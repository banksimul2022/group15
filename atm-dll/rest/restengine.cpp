#include "restengine.h"
#include <QDebug>
#include <QDir>
#include <QJsonObject>
#include <QNetworkReply>
#include <QVariant>
#include <QJsonDocument>
#include <QDebug>
#include "resterrorcode.h"


RESTEngine::RESTEngine(QString endpointstr, QObject *parent) : RESTEngine(QUrl(endpointstr),parent)
{}



RESTEngine::RESTEngine(QUrl endpointstr, QObject *parent) :
    RESTInterface(parent),
    apiEndpoint(endpointstr),
    token(nullptr),
    nextOffset(0),
    prevOffset(0)


{
    this->Manager=new QNetworkAccessManager(this);
    connect(this->Manager,&QNetworkAccessManager::finished,this,&RESTEngine::replySlot);



}



void RESTEngine::login(QString cardNumber, QString pin)
{

    QNetworkRequest request = this->createRequest("/api/login",RestReturnData::typeLogin,"application/json"); //post pyyntö käyttää "application/json"
    QJsonObject Jsonobj;
    Jsonobj.insert("card_number",cardNumber);
    Jsonobj.insert("pin",pin);

    this->Manager->post(request,QJsonDocument(Jsonobj).toJson());

}

void RESTEngine::logout()
{
 this->token=nullptr;
    this->nextOffset=0;
    this->prevOffset=0;

    emit dataReturn(new RestReturnData(RestReturnData::typeLogout,-1));



}

void RESTEngine::getInfo()
{
    QNetworkRequest request = this->createRequest("/api/info",RestReturnData::typeInfo);

    this->Manager->get(request);
}

void RESTEngine::latestTransaction(int count)
{
    QUrlQuery query;
    query.addQueryItem("count",QString::number(count));
    QNetworkRequest request = this->createRequest("/api/transactions/latest",query,RestReturnData::typeLatestTransaction);
    this->Manager->get(request);
}

void RESTEngine::nextTransactions(int count)
{
    QUrlQuery query;
    query.addQueryItem("offset",QString::number(this->nextOffset));
    query.addQueryItem("count",QString::number(count));
    QNetworkRequest request = this->createRequest("/api/transactions/forward",query,RestReturnData::typeTransaction);
    this->Manager->get(request);
}

void RESTEngine::prevTransactions(int count)
{
    QUrlQuery query;
    query.addQueryItem("offset",QString::number(this->prevOffset));
    query.addQueryItem("count",QString::number(count));
    QNetworkRequest request = this->createRequest("/api/transactions/back",query,RestReturnData::typeTransaction);
    this->Manager->get(request);

}

void RESTEngine::resetTransactionPageIndex()
{
    this->nextOffset=0;
    this->prevOffset=0;
}


void RESTEngine::withdraw(double sum, bool useCredit) //muuta sum
{
    QNetworkRequest request = this->createRequest("/api/withdraw",RestReturnData::typeWithdraw,"application/json"); //post pyyntö käyttää "application/json"
    QJsonObject Jsonobj;
    Jsonobj.insert("sum",sum);
    Jsonobj.insert("credit",useCredit);

    this->Manager->post(request,QJsonDocument(Jsonobj).toJson());

}

void RESTEngine::deposit(double sum)
{
    QNetworkRequest request = this->createRequest("/api/deposit",RestReturnData::typeDeposit,"application/json"); //post pyyntö käyttää "application/json"
    QJsonObject Jsonobj;
    Jsonobj.insert("sum",sum);

    this->Manager->post(request,QJsonDocument(Jsonobj).toJson());

}

void RESTEngine::showBalance()
{
    QNetworkRequest request = this->createRequest("/api/balance",RestReturnData::typeBalance);

    this->Manager->get(request);
}
QNetworkRequest RESTEngine::createRequest(QString path,  RestReturnData::ReturnType type, QString contentType)
{
    QUrl url (this->apiEndpoint);
    url.setPath(QDir::cleanPath(this->apiEndpoint.path()+path)); //+"/"+path

    return this->createRequest(url,type,contentType);
}
QNetworkRequest RESTEngine::createRequest(QString path, QUrlQuery params, RestReturnData::ReturnType type, QString contentType)
{
    QUrl url (this->apiEndpoint); //url()
    url.setPath(QDir::cleanPath(this->apiEndpoint.path()+path)); //+"/"+path
    url.setQuery(params);
    return this->createRequest(url,type,contentType);
}

void RESTEngine::transfer(QString accountNumber, double sum)
{
    QNetworkRequest request = this->createRequest("/api/transfer", RestReturnData::typeTransfer,"application/json"); //post pyyntö käyttää "application/json"
    QJsonObject Jsonobj;
    Jsonobj.insert("accountNumber",accountNumber);
    Jsonobj.insert("sum",sum);

    this->Manager->post(request,QJsonDocument(Jsonobj).toJson());

}

QNetworkRequest RESTEngine::createRequest(QUrl url, RestReturnData::ReturnType type, QString contentType) //creates web request
{
    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::User,QVariant(type));
    if (this->token !=nullptr){
        request.setRawHeader("Authorization",this->token);
    }
    if (contentType !=nullptr){
        request.setHeader(QNetworkRequest::ContentTypeHeader,contentType); //post/put
    }

    return request;
}


void RESTEngine::replySlot(QNetworkReply *reply)
{

    QVariant variant = reply->request().attribute(QNetworkRequest::User);
    if (!variant.canConvert<int>()){
        emit dataReturn(new RestReturnData(RestReturnData::typeinternalerror, RestErrors::ERR_UNTAGGED_REPLY));

        return;
    }
    if (reply->error()!=QNetworkReply::NoError && reply->error() < QNetworkReply::ProxyConnectionRefusedError ){
         emit dataReturn(new RestReturnData(RestReturnData::typeinternalerror, RestErrors::ERR_NETWORK));
        return;
}

    QJsonDocument jsonDoc;
    QJsonObject jsonObj;
    int type = variant.value<int>();

    if(type != RestReturnData::typeBalance) {
        QByteArray responseData = reply->readAll();
        jsonDoc = QJsonDocument::fromJson(responseData);
        jsonObj = jsonDoc.object();
      }


    int error = jsonObj.value("error").toInt(-1); //  error oletus arvo -1(kaikki ok)

    switch (type) {
        case RestReturnData::typeLogin:

            if(error == -1){
                this->token = ("Bearer "+jsonObj.value("token").toString()).toUtf8(); // Luetaan JSonobjectista token arvo ja muutetaan se string->QByteArray utf8 muodossa
                 }
                emit dataReturn(new RestReturnData(RestReturnData::typeLogin,error));
                return;
        case RestReturnData::typeInfo: {
                 this->lastData = jsonObj;
                 this->lastError = error;
                QNetworkRequest request = this->createRequest(jsonObj.value("profile").toString(),RestReturnData::typeProfile);
                this->Manager->get(request);

                return; }
    case RestReturnData::typeProfile: {
        emit dataReturn(new RestInfoData(&this->lastData, reply, this->lastError, error));
        return;

    }

       case RestReturnData::typeDeposit:

        emit dataReturn(new RestReturnData(RestReturnData::typeDeposit,error));
                return;
       case RestReturnData::typeWithdraw:
        emit dataReturn(new RestReturnData(RestReturnData::typeWithdraw,error));
                return;
       case RestReturnData::typeLatestTransaction:
        emit dataReturn(new RestTransactionData(&jsonObj,RestReturnData::typeLatestTransaction,error));
                return;
       case RestReturnData::typeTransaction: {
            RestTransactionData *data = new RestTransactionData(&jsonObj,RestReturnData::typeTransaction,error);

            this->nextOffset=jsonObj.value("nextOffset").toInt(0);
            this->prevOffset=jsonObj.value("prevOffset").toInt(0);

           emit dataReturn(data);
                return; }
       case RestReturnData::typeBalance: {
            emit dataReturn(new RestBalanceData(&jsonObj,error));
                return;
            }
    case RestReturnData::typeTransfer: {
        emit dataReturn(new RestReturnData(RestReturnData::typeTransfer,error));
        return;
    }

    }


    emit dataReturn(new RestReturnData(RestReturnData::typeinternalerror, RestErrors::ERR_UNKNOW_REPLY));
}
