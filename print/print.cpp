#include "print.h"
#include <QTextDocument>
#include <QPrinter>
#include <QFileDialog>
#include <QTextBlock>
#include <QPrintPreviewDialog>
#include "control/controller.h"
Print::Print(QObject *parent) : QObject(parent)
{
    m_thread = new QThread;

    this->moveToThread(m_thread);
   m_thread->start();

    connect(this,&Print::startSaveToPdf,this,[&](const QString&valueStr,const QString &fileName)
    {
        QPrinter printer(QPrinter::HighResolution);
        printer.setPageSize(QPageSize(QPageSize::A4));
        printer.setOutputFormat(QPrinter::PdfFormat);


        QString curFileName;
        if(!fileName.endsWith(".pdf"))
        {
            curFileName= fileName+".pdf";
        }
        curFileName= Controller::instance()->fileNameFromQml(fileName);

        printer.setOutputFileName(curFileName);
        QTextDocument textDocument;
        textDocument.setHtml(valueStr);
        textDocument.print(&printer);
        textDocument.end();
    });
}

void Print::saveToPdf(const QList<QString> &roleNameList, const QList<QVariant> &valueList,const QString&fileName)
{

    QString curFileValue = createHtml(roleNameList,valueList,580);
    emit startSaveToPdf(curFileValue,fileName);

   // emit startSaveToPdf(roleNameList,valueList,fileName);

}

QString Print::createHtml(const QList<QString> &roleNameList, const QList<QVariant> &valueList,int tableSize)
{
    int roleSize = roleNameList.size();
    int valueSize = valueList.size();
    QString html;
    html+=QString("<table width=\"%1\" border=\"1\" align=\"center\">").arg(tableSize);
    html+="<tr style=\"background-color:white\">";
    for(int i=0;i<roleSize;i++)
    {
        html += QString("<th>%1</th>").arg(roleNameList.at(i));
    }

    if(!roleNameList.isEmpty())
    {
        if((valueSize>=roleSize)&&(valueSize%roleSize==0))
        {
            for(int j=0;j<valueSize/roleSize;j++)
            {
                html+="<tr>";
                for(int i=0;i<roleSize;i++)
                {
                    html+=QString("<td>%1</td>").arg(valueList.at(roleSize*j+i).toString());
                }
                html +="</tr>";

            }
        }
    }
    html+="</table><br/>";
    return html;
}

void Print::startPrint(const QList<QString> &roleNameList, const QList<QVariant> &valueList)
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setPageSize(QPageSize(QPageSize::A4));
    QTextDocument textDocument;
    textDocument.setHtml(createHtml(roleNameList,valueList,580));
    textDocument.print(&printer);
    textDocument.end();
}

void Print::printPreview(const QList<QString> &roleNameList, const QList<QVariant> &valueList)
{
    QPrinter printer(QPrinter::HighResolution);
    //纸张大小
    printer.setPageSize(QPageSize(QPageSize::A4));
    QPrintPreviewDialog previewDialog(&printer, nullptr);
    previewDialog.setWindowFlags(Qt::WindowStaysOnTopHint|Qt::WindowMaximizeButtonHint|Qt::MSWindowsFixedSizeDialogHint|Qt::WindowCloseButtonHint);
    previewDialog.setMinimumSize(960,720);
    connect(&previewDialog,&QPrintPreviewDialog::paintRequested,this,[=](QPrinter *printer)
    {
        QTextDocument textDocument;
        textDocument.setHtml(createHtml(roleNameList,valueList,580));
        textDocument.print(printer);
        textDocument.end();
    });
    previewDialog.exec();

}
