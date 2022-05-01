#include "restengine.h"
#include "restinterface.h"


RESTInterface *RESTInterface::createInstance(QSettings *settings)
{
   // return new RESTEngine("http://localhost:3000"); //annetaan RESTEngine instanssille endpointurl
    return new RESTEngine(settings->value("endpoint-url",
                          QVariant(QStringLiteral("http://localhost:3000"))).toString()); //Engine instanssille endpoint url. luetaan endpointurl asetus
}

RESTInterface::RESTInterface(QObject *parent) : QObject(parent)
{

}

RESTInterface::~RESTInterface()
{

}
