#include "abstractlink.h"
#include <QThread>
#include <QDebug>
AbstractLink::AbstractLink(QObject *parent) : QObject(parent)
{

}

AbstractLink::~AbstractLink()
{

}

void AbstractLink::sendData(const QByteArray &array)
{
    emit writeData(array);
}

void AbstractLink::connectLink()
{
  emit startConnect();
}

void AbstractLink::disconnectLink()
{
    emit stopConnect();
}




