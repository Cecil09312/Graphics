#include "print.h"
#include <QTextDocument>
#include <QPrinter>
#include <QFileDialog>
#include <QTextBlock>
#include <QPrintPreviewDialog>
Print::Print(QObject *parent) : QObject(parent)
{

}

void Print::saveToPdf(QList<QString> roleNameList, QList<QVariant> valueList)
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setOutputFormat(QPrinter::PdfFormat);

    QString fileName=   QFileDialog::getSaveFileName(nullptr, tr("保存PDF文件"), QString(), "*.pdf");
    if(!fileName.isEmpty())
    {
        printer.setOutputFileName(fileName);
        QTextDocument text_document;
        text_document.setHtml(createHtml(roleNameList,valueList,650));
        text_document.print(&printer);
        text_document.end();
    }
}

QString Print::createHtml(QList<QString> roleNameList, QList<QVariant> valueList,int tableSize)
{
    int roleSize = roleNameList.size();
    int valueSize = valueList.size();
    QString html;
    html+=QString("<table width=\"%1\"border=\"1\"align=\"center\">").arg(tableSize);
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

void Print::startPrint(QList<QString> roleNameList, QList<QVariant> valueList)
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setPageSize(QPageSize(QPageSize::A4));
    QTextDocument text_document;
    text_document.setHtml(createHtml(roleNameList,valueList,650));
    text_document.print(&printer);
    text_document.end();
}

void Print::printPreview(QList<QString> roleNameList, QList<QVariant> valueList)
{
    QPrinter printer(QPrinter::HighResolution);
    //自定义纸张大小
    printer.setPageSize(QPrinter::Custom);

    QPrintPreviewDialog previewDialog(&printer, nullptr);
    previewDialog.setMinimumSize(800,600);
    connect(&previewDialog,&QPrintPreviewDialog::paintRequested,this,[=](QPrinter *printer)
    {
        QTextDocument text_document;
        text_document.setHtml(createHtml(roleNameList,valueList,650));
        text_document.print(printer);
        text_document.end();
    });
    previewDialog.exec();

}
