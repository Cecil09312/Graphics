#include "itemiconinfotojson.h"
#include <QDebug>

QHash<int,QString> ItemIconInfoToJson::s_iconIndexHash = QHash<int,QString>();
int ItemIconInfoToJson::s_currentIconIndex =0;

QHash<QString,QVariant>ItemIconInfoToJson::m_iconInfoHash=QHash<QString,QVariant>();
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

QVariant ItemIconInfoToJson::readValueFromKey(const QString &key)
{

    return m_iconInfoHash.value(key);
}

QString ItemIconInfoToJson::getValue(const QString &index, const QString &key)
{
    QHash<QString,QVariant>valueHash =readValueFromKey(index).toHash();
    if(valueHash.contains(key))
    {
        return valueHash.value(key).toString();
    }
    else
    {
        return "";
    }
}

int ItemIconInfoToJson::sizeOfHash()
{
    return m_iconInfoHash.size();
}

QHash<QString, QVariant> ItemIconInfoToJson::getIconInfoHash()
{
    //Hash<QString, QVariant>iconInfoHash =readFile(c_filePath).toHash();
    return m_iconInfoHash;
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

int ItemIconInfoToJson::iconIndex(const QString &iconName)
{
    int curIndex = 0;
    foreach (QString value, s_iconIndexHash.values())
    {
        if(value.contains(iconName))
        {
            curIndex=  s_iconIndexHash.key(value);
        }
    }
    return curIndex;
}

void ItemIconInfoToJson::setCurrentIconIndex(int index)
{
    s_currentIconIndex = index;
}

void ItemIconInfoToJson::setOtherInfoHash(int pos, const QString &itemName, QVariant value)
{
    m_otherInfoHash[pos][itemName] = value;
}

QVariant ItemIconInfoToJson::otherInfo(int pos, const QString &itemName)
{
    return m_otherInfoHash[pos][itemName];
}

void ItemIconInfoToJson::initIconInfo()
{
    m_iconInfoHash = readFile(c_filePath).toHash();
    QList<QString>keyList= m_iconInfoHash.keys();
    foreach (QString key, keyList) {
        QHash<QString,QVariant>iconHash=  m_iconInfoHash.value(key).toHash();
        s_iconIndexHash[key.toInt()]=iconHash.value("imagePath").toString();
    }
}


