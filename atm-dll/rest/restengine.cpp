#include "restengine.h"
#include <QDebug>
#include <QDir>
#include <QJsonObject>
#include <QNetworkReply>
#include <QVariant>
#include <QJsonDocument>
#include <QDebug>


RESTEngine::RESTEngine(QString endpointstr, QObject *parent) : RESTEngine(QUrl(endpointstr),parent)
{}



RESTEngine::RESTEngine(QUrl endpointstr, QObject *parent) :
    RESTInterface(parent),
    apiEndpoint(endpointstr),
    token(nullptr),
    offset(0),
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
    this->offset=0;
    this->prevOffset=0;

    emit dataReturn(new RestReturnData(RestReturnData::typeLogout,-1));



}

void RESTEngine::getInfo()
{
    QNetworkRequest request = this->createRequest("/api/info",RestReturnData::typeInfo);

    this->Manager->get(request);
}

void RESTEngine::nextTransactions(int count)
{
    QUrlQuery query;
    query.addQueryItem("offset",QString::number(this->offset));
    query.addQueryItem("count",QString::number(count));
    QNetworkRequest request = this->createRequest("/api/transactions",RestReturnData::typeTransaction);
    this->Manager->get(request);
}

void RESTEngine::prevTransactions(int count)
{
    QUrlQuery query;
    query.addQueryItem("offset",QString::number(this->prevOffset));
    query.addQueryItem("count",QString::number(count));
    QNetworkRequest request = this->createRequest("/api/transactions",RestReturnData::typeTransaction);
    this->Manager->get(request);

}


void RESTEngine::withdraw(double sum) //muuta sum
{
    QNetworkRequest request = this->createRequest("/api/withdraw",RestReturnData::typeWithdraw,"application/json"); //post pyyntö käyttää "application/json"
    QJsonObject Jsonobj;
    Jsonobj.insert("sum",sum);

    this->Manager->post(request,QJsonDocument(Jsonobj).toJson());

}

void RESTEngine::deposit(double sum)
{
    QNetworkRequest request = this->createRequest("/api/deposit",RestReturnData::typeDeposit,"application/json"); //post pyyntö käyttää "application/json"
    QJsonObject Jsonobj;
    Jsonobj.insert("sum",sum);

    this->Manager->post(request,QJsonDocument(Jsonobj).toJson());

}
QNetworkRequest RESTEngine::createRequest(QString path,  RestReturnData::ReturnType type, QString contentType) //creates web request
{
    QUrl url (this->apiEndpoint); //url()
    url.setPath(QDir::cleanPath(this->apiEndpoint.path()+path)); //+"/"+path

    return this->createRequest(url,type,contentType);
}
QNetworkRequest RESTEngine::createRequest(QString path, QUrlQuery params, RestReturnData::ReturnType type, QString contentType) //creates web request
{
    QUrl url (this->apiEndpoint); //url()
    url.setPath(QDir::cleanPath(this->apiEndpoint.path()+path)); //+"/"+path
    url.setQuery(params);
    return this->createRequest(url,type,contentType);
}

QNetworkRequest RESTEngine::createRequest(QUrl url, RestReturnData::ReturnType type, QString contentType) //creates web request
{
    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::User,QVariant(type));
    if (this->token !=nullptr){
        request.setRawHeader("Authorization",this->token); //
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
        return;
    }

    QByteArray responseData = reply->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);

    QJsonObject jsonObj = jsonDoc.object();
    int error = jsonObj.value("error").toInt(-1); //  error oletus arvo -1(kaikki ok)

    switch (variant.value<int>()) {
        case RestReturnData::typeLogin:

            if(error == -1){
                this->token = ("Bearer "+jsonObj.value("token").toString()).toUtf8(); // Luetaan JSonobjectista token arvo ja muutetaan se string->QByteArray utf8 muodossa
                 }
                emit dataReturn(new RestReturnData(RestReturnData::typeLogin,error));
            break;
        case RestReturnData::typeInfo:
                emit dataReturn(
                  new RestInfoData(
                    &jsonObj,
                    error
                )
             );
            break;
       case RestReturnData::typeDeposit:

        emit dataReturn(new RestReturnData(RestReturnData::typeDeposit,error));
            break;
       case RestReturnData::typeWithdraw:
        emit dataReturn(new RestReturnData(RestReturnData::typeWithdraw,error));
            break;
       case RestReturnData::typeTransaction:
            RestTransactionData *data = new RestTransactionData(&jsonObj,error);
            this->prevOffset=this->offset;
            this->offset=data->getNextOffSet();
           emit dataReturn(data);
            break;

    }


}
