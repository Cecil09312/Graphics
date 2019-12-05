#ifndef ITEMICONINFOTOJSON_H
#define ITEMICONINFOTOJSON_H
#include "qmlforjson.h"

class ItemIconInfoToJson:public QmlForJson
{
    Q_OBJECT
public:
    ItemIconInfoToJson(QObject *parent = nullptr);
    ~ItemIconInfoToJson();
    Q_INVOKABLE  void saveItemIconInfo(const QString &index, const QString &key, const QVariant &value);
    Q_INVOKABLE  void itemIconInfoToJson();
    Q_INVOKABLE  void removeIconInfo(const QString &index);
    Q_INVOKABLE  void clearIconInfo();
    Q_INVOKABLE  QString readFileFromJson();
    Q_INVOKABLE  QVariant readValueFromKey(const QString &key);
    Q_INVOKABLE  QString getValue(const QString &index,const QString &key);
    Q_INVOKABLE  int sizeOfHash();
    QHash<QString, QVariant> getIconInfoHash();
    Q_INVOKABLE static void setIconIndexHash(int index,const QString &iconName );
    Q_INVOKABLE static QString getIconName(int index);
    Q_INVOKABLE static void removeIconIndex(int index);
    Q_INVOKABLE static void clearIconIndex();
    Q_INVOKABLE static int currentIconIndex();
    Q_INVOKABLE static int iconIndex(const QString &iconName);
    Q_INVOKABLE static void setCurrentIconIndex(int index);
    Q_INVOKABLE void setOtherInfoHash(int pos ,const QString&itemName,QVariant value);
    Q_INVOKABLE QVariant otherInfo(int pos ,const QString&itemName);


private:
    QHash<QString,QVariant>m_iconInfoHash;
    const QString c_filePath = QCoreApplication::applicationDirPath()+"/iconInfo.json";
    static QHash<int,QString> s_iconIndexHash;
    QHash<int ,QHash<QString,QVariant> >m_otherInfoHash;
    static int s_currentIconIndex;
};

#endif // ItemIconInfoToJson_H
