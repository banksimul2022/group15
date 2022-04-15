#include "restengine.h"
#include <QDebug>
#include <QDir>
#include <QJsonObject>
#include <QNetworkReply>
#include <QVariant>
#include <QJsonDocument>
#include <QDebug>


RESTEngine::RESTEngine(QString endpointstr, QObject *parent) : RESTEngine(QUrl(endpointstr),parent)
{} //älä koske



RESTEngine::RESTEngine(QUrl endpointstr, QObject *parent) :
    RESTInterface(parent),
    apiEndpoint(endpointstr),
    token(nullptr)
{
    this->Manager=new QNetworkAccessManager;
    connect(this->Manager,&QNetworkAccessManager::finished,this,&RESTEngine::replySlot);



}

RESTEngine::~RESTEngine()
{
    qDebug()<<"tuhoutuu";
}

void RESTEngine::login(QString cardNumber, QString pin)
{

    QNetworkRequest request = this->createRequest("/api/login",RestReturnData::typeLogin,"application/json"); //login pyyntö
    QJsonObject Jsonobj;
    Jsonobj.insert("card_number",cardNumber);
    Jsonobj.insert("pin",pin);

    this->Manager->post(request,QJsonDocument(Jsonobj).toJson());

}

void RESTEngine::logout()
{
 this->token=nullptr;
    emit dataReturn(new RestReturnData(RestReturnData::typeLogout,-1));
}

void RESTEngine::getInfo()
{
    QNetworkRequest request = this->createRequest("/api/info",RestReturnData::typeInfo); //login pyyntö


    this->Manager->get(request);
}

QNetworkRequest RESTEngine::createRequest(QString path,RestReturnData::ReturnType type, QString contentType) //creates web request
{

    QUrl url (this->apiEndpoint); //url()
    url.setPath(QDir::cleanPath(this->apiEndpoint.path()+path)); //+"/"+path
    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::User,QVariant(type));
    if (this->token !=nullptr){
        request.setRawHeader("Authorization",this->token); //(QByteArray("Authorization"),(myToken));
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
    int error = jsonObj.value("error").toInt(-1);

    switch (variant.value<int>()) {
        case RestReturnData::typeLogin:

            if(error == -1){
                this->token = ("Bearer "+jsonObj.value("token").toString()).toUtf8(); // Luetaan JSonobjectista token arvo ja muutetaan se string->QByteArray utf8 muodossa
                 }
                emit dataReturn(new RestReturnData(RestReturnData::typeLogin,error)); //  error oletus arvo -1(kaikki ok)
            break;
        case RestReturnData::typeInfo:
             emit dataReturn(
                new RestInfoData(
                    jsonObj.value("fName").toString(),
                    jsonObj.value("lName").toString(),
                    jsonObj.value("accountNumber").toString(),
                    error
                )
             );
        break;

    }


}
