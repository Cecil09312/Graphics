#include "abstractlink.h"
#include <QThread>
#include <QDebug>
#include "control/controller.h"
AbstractLink::AbstractLink(QObject *parent) : QObject(parent)
{

}

AbstractLink::~AbstractLink()
{

}

QIODevice *AbstractLink::device()
{
    return nullptr;
}

qint64 AbstractLink::writeDataSize()
{
    return 0;
}

bool AbstractLink::writeDataSuccess()
{
    return false;
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

void AbstractLink::sendAll()
{
    emit  sendAllData();
}




