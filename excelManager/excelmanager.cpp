#include "excelmanager.h"
#include <QVariant>
#include <QDebug>
ExcelManager::ExcelManager(QObject *parent)
    : QObject(parent)
{

#ifdef Q_OS_WIN
    m_worksheets=nullptr;
    m_workbook=nullptr;
    m_excel = new QAxObject("Excel.Application",this);
    m_excel->dynamicCall("SetVisible(bool Visble)", "false");
    m_excel->setProperty("DisplayAlerts", false);
    m_workbooks = m_excel->querySubObject("WorkBooks");

#endif
}

ExcelManager::~ExcelManager()
{

}

bool ExcelManager::openExcel(const QString &fileName)
{
#ifdef Q_OS_WIN
    m_workbook = m_workbooks->querySubObject("Open(const QString&)", fileName);
    m_worksheets = m_workbook->querySubObject("Sheets");
    int count= m_worksheets->property("Count").toInt();
    if(count>0)
    {
        return true;
    }
    else
    {
        return false;
    }
#elif defined(Q_OS_LINUX)
    Q_UNUSED(fileName);
    return false;

#endif
}

QVariant ExcelManager::readExcel( const QString&sheetName)
{

    QVariant var;
#ifdef Q_OS_WIN

    if(m_worksheets==nullptr)
    {
        return var;
    }
    QAxObject *worksheet = m_worksheets->querySubObject("Item(const QString&)", sheetName);

    if (worksheet != nullptr && ! worksheet->isNull())
    {
        QAxObject *usedRange = worksheet->querySubObject("UsedRange");
        if(nullptr == usedRange || usedRange->isNull())
        {
            return var;
        }
        var = usedRange->dynamicCall("Value");
        delete usedRange;
    }
#elif defined (Q_OS_LINUX)
    Q_UNUSED(sheetName);
#endif
    return var;

}

void ExcelManager::closeExcel()
{
#ifdef Q_OS_WIN
    if(m_workbook!=nullptr)
    {
        m_workbook->dynamicCall("Close(Boolean)", true);
    }
#endif
}

void ExcelManager::quitExcel()
{
#ifdef Q_OS_WIN
    if(m_excel!=nullptr)
    {
        m_excel->dynamicCall("Quit()");
    }
#endif
}
