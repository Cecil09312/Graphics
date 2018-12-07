#ifndef QMLFORJSON_H
#define QMLFORJSON_H

#include <QObject>
#include <QFile>
#include <QFuture>
#include <QtConcurrent>
#include <QCoreApplication>
#include <QApplication>

class QmlForJson : public QObject
{
    Q_OBJECT
   // Q_PROPERTY(QString fileName READ fileName WRITE setFileName)
public:
    explicit QmlForJson(QObject *parent = nullptr);
    Q_INVOKABLE static void writeFile(const QVariant &value, const QString&fileName=QCoreApplication::applicationDirPath()+"/architePlan.json");
    Q_INVOKABLE static QVariant readFile(const QString&fileName=QCoreApplication::applicationDirPath()+"/architePlan.json");
    Q_INVOKABLE static QString readFileToString(const QString&fileName=QCoreApplication::applicationDirPath()+"/architePlan.json");

   // void setFileName(const QString&name);
    //QString fileName() const;
signals:

public slots:
private:
    //QString m_fileName;
   //static QString s_filePath;

};

#endif // QMLFORJSON_H
