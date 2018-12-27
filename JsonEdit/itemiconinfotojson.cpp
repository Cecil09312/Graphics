#include "itemiconinfotojson.h"
#include <QDebug>

QHash<int,QString> ItemIconInfoToJson::s_iconIndexHash = QHash<int,QString>();
int ItemIconInfoToJson::s_currentIconIndex =0;
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

void ItemIconInfoToJson::removeIconInfo(const QString &index)
{
    m_iconInfoHash.remove(index);
}

void ItemIconInfoToJson::clearIconInfo()
{
    m_iconInfoHash.clear();
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
    QString iconStr= iconName;
    if(iconName.startsWith("qrc"))
    {
      iconStr=  iconStr.section("qrc",1,1);

    }
    s_iconIndexHash[index] = iconStr;
}

QString ItemIconInfoToJson::getIconName(int index)
{
    if(index>=0)
    {
        return s_iconIndexHash[index];
    }
    else
    {
        return "";
    }
}

void ItemIconInfoToJson::removeIconIndex(int index)
{
    s_iconIndexHash.remove(index);
}

void ItemIconInfoToJson::clearIconIndex()
{
    s_iconIndexHash.clear();
}

int ItemIconInfoToJson::currentIconIndex()
{
    return s_currentIconIndex;
}

void ItemIconInfoToJson::setCurrentIconIndex(int index)
{
    s_currentIconIndex = index;
}


