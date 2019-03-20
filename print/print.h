#ifndef PRINT_H
#define PRINT_H

#include <QObject>

class Print : public QObject
{
    Q_OBJECT
public:
    explicit Print(QObject *parent = nullptr);
    void saveToPdf(QList<QString>roleNameList, QList<QVariant>valueList);
    QString createHtml(QList<QString> roleNameList, QList<QVariant> valueList,int tableSize);
    void startPrint(QList<QString>roleNameList, QList<QVariant>valueList);
    void printPreview(QList<QString>roleNameList, QList<QVariant>valueList);

signals:

public slots:
};

#endif // PRINT_H
