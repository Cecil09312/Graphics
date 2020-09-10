#ifndef PRINT_H
#define PRINT_H

#include <QObject>
#include <QThread>
#include <QPrintPreviewDialog>
#include <QPrinter>
#include <QTextDocument>
#include <QPrintPreviewWidget>
#include <QProcess>
#include <QPrintDialog>
class Print : public QObject
{
    Q_OBJECT
    //Q_DECLARE_METATYPE(QVariant)
public:
    static Print *instance();
    ~Print();
    void saveToPdf(const QList<QString> &roleNameList, const QList<QVariant> &valueList, const QString &fileName);
    QString createHtml(const QList<QString> &roleNameList, const QList<QVariant> &valueList, int tableSize,int size=10);
     QString createAllHtml(const QList<QString> &roleNameList, const QList<QVariant> &valueList, int tableSize);
    void startPrint(const QList<QString>&roleNameList, const QList<QVariant>&valueList);
    void printPreview(const QList<QString>&roleNameList, const QList<QVariant>&valueList);
    bool  saveDataToPdf(const QList<QString> &roleNameList, const QList<QVariant> &valueList,const QString&fileName);
   // void saveFileToPdf(const QString &fileName);
   // void saveFileToPdf(QPrinter*printer,const QString &fileName);

signals:
   void startSaveToPdf(const QList<QString> &roleNameList, const QList<QVariant> &valueList,const QString&fileName);
   void closeProcess();
   void startPrintFile(const QString &printName,int min,int max, const QString&fileName);
   void startPrintEntireFile(const QString &printName,const QString&fileName);
   void printerDataSaveToPdf(const QList<QString> &roleNameList, const QList<QVariant> &valueList,const QString&fileName,const QString &printName,int min,int max);
   void printerDataAllToPdf(const QList<QString> &roleNameList, const QList<QVariant> &valueList,const QString&fileName,const QString &printName);


public slots:
private:
   explicit Print(QObject *parent = nullptr);
private:
    QThread *m_thread;
    QPrinter *m_printer;
  //  QTextDocument *m_document;
   // QTextDocument *m_threadDocument;
    static QSharedPointer<Print>m_print;
    //QPrintPreviewDialog *m_printPreviewDialog;
    QProcess *m_process;
    QString m_dataStr;
    QPrintDialog *m_printDialog;
    QString m_pdfDir;
    QList<QString>m_roleNameList;
    QList<QVariant>m_valueList;
};

#endif // PRINT_H
