#include "restengine.h"
#include "restinterface.h"


RESTInterface *RESTInterface::createInstance()
{
    return new RESTEngine("http://localhost:3000"); //annetaan RESTEngine instanssille endpointurl
}

RESTInterface::RESTInterface(QObject *parent) : QObject(parent)
{

}

RESTInterface::~RESTInterface()
{

}
