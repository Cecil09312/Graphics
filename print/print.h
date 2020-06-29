#ifndef PRINT_H
#define PRINT_H

#include <QObject>
#include <QThread>

class Print : public QObject
{
    Q_OBJECT
    //Q_DECLARE_METATYPE(QVariant)
public:
    explicit Print(QObject *parent = nullptr);
    void saveToPdf(const QList<QString> &roleNameList, const QList<QVariant> &valueList, const QString &fileName);
    QString createHtml(const QList<QString> &roleNameList, const QList<QVariant> &valueList, int tableSize);
    void startPrint(const QList<QString>&roleNameList, const QList<QVariant>&valueList);
    void printPreview(const QList<QString>&roleNameList, const QList<QVariant>&valueList);

signals:
   void startSaveToPdf(const QString &valueStr,const QString &fileName);
public slots:
private:
    QThread *m_thread;
};

#endif // PRINT_H
