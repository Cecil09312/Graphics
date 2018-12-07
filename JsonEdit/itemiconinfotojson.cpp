#include "itemiconinfotojson.h"
#include <QDebug>

QHash<int,QString> ItemIconInfoToJson::s_iconIndexHash = QHash<int,QString>();
ItemIconInfoToJson::ItemIconInfoToJson(QObject *parent):
    QmlForJson(parent)
{

}

ItemIconInfoToJson::~ItemIconInfoToJson()
{

}

void ItemIconInfoToJson::saveItemIconInfo(const QString &index, const QString &key, const QVariant &value)
{

    QHash<QString,QVariant> valueHash= m_iconInfoHash[index].toHash();
    valueHash[key] = value;
    m_iconInfoHash[index] = valueHash;
}

void ItemIconInfoToJson::itemIconInfoToJson()
{
    writeFile(m_iconInfoHash,c_filePath);

}

QString ItemIconInfoToJson::readFileFromJson()
{
    return readFileToString(c_filePath);
}

int ItemIconInfoToJson::sizeOfHash()
{
    return getIconInfoHash().size();
}

QHash<QString, QVariant> ItemIconInfoToJson::getIconInfoHash()
{
   QHash<QString, QVariant>iconInfoHash =readFile(c_filePath).toHash();
   return iconInfoHash;
}

void ItemIconInfoToJson::setIconIndexHash(int index, const QString &iconName)
{
    s_iconIndexHash[index] = iconName;
}

QString ItemIconInfoToJson::getIconName(int index)
{
    return s_iconIndexHash[index];
}

void ItemIconInfoToJson::removeIconIndex(int index)
{
    s_iconIndexHash.remove(index);
}

void ItemIconInfoToJson::clearIconIndex()
{
    s_iconIndexHash.clear();
}


