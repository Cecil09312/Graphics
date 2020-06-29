#include "qmlforjson.h"
#include <QJsonDocument>
#include <QVariant>
#include <QFutureWatcher>
//QString QmlForJson::s_filePath = QApplication::applicationDirPath()+"/";
QmlForJson::QmlForJson(QObject *parent)
    : QObject(parent)
{

}

void QmlForJson::writeFile(const QVariant &value,const QString &fileName)
{

    QFuture <void > future = QtConcurrent::run([=]()
    {
        QFile file(fileName);

        if(file.open(QIODevice::WriteOnly))
        {
            file.write(QJsonDocument::fromVariant(value).toJson());
            file.close();
        }

    });

   future.waitForFinished();
}

QVariant QmlForJson::readFile(const QString &fileName)
{
    QVariant value;

    QFuture <void > future = QtConcurrent::run([&]()
    {
        QFile file(fileName);
        if(file.exists())
        {
            if(file.open(QIODevice::ReadOnly))
            {
                value = QJsonDocument::fromJson(file.readAll()).toVariant();
                file.close();
            }
        }


    });
    future.waitForFinished();
    return value;
}

QString QmlForJson::readFileToString(const QString &fileName)
{
    static QString value = QString();
    QFuture <void > future = QtConcurrent::run([&]()
    {
        QFile file(fileName);
        if(file.exists())
        {
            if(file.open(QIODevice::ReadOnly))
            {
                value =QString(file.readAll()) ;
                file.close();
            }
        }


    });
    future.waitForFinished();
    return  value;
}

//void QmlForJson::setFileName(const QString &name)
//{
//    m_fileName = name;
//}

//QString QmlForJson::fileName() const
//{
//    return m_fileName;
//}
