#ifndef EXCELMANAGER_H
#define EXCELMANAGER_H

#include <QObject>
#ifdef Q_OS_WIN
#include <QAxObject>
#endif
class ExcelManager : public QObject
{
    Q_OBJECT
public:
    explicit ExcelManager(QObject *parent = nullptr);
    ~ExcelManager();
    bool openExcel(const QString&fileName);
    QVariant readExcel(const QString &sheetName);
    void closeExcel();
    void quitExcel();

signals:

public slots:
private:
#ifdef Q_OS_WIN
    QAxObject *m_excel;
    QAxObject *m_workbooks;
    QAxObject *m_worksheets;
    QAxObject *m_workbook;

#endif


};

#endif // EXCELMANAGER_H
