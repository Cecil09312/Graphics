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
    Q_INVOKABLE  QString readFileFromJson();
    Q_INVOKABLE  int sizeOfHash();
    QHash<QString, QVariant> getIconInfoHash();
    Q_INVOKABLE static void setIconIndexHash(int index,const QString &iconName );
    Q_INVOKABLE static QString getIconName(int index);
    Q_INVOKABLE static void removeIconIndex(int index);
    Q_INVOKABLE static void clearIconIndex();
private:
    QHash<QString,QVariant>m_iconInfoHash;

    const QString c_filePath = QCoreApplication::applicationDirPath()+"/iconInfo.json";
    static QHash<int,QString> s_iconIndexHash;

};

#endif // ItemIconInfoToJson_H
