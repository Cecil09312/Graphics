#include "print.h"


#include <QFileDialog>
#include <QTextBlock>

#include "control/controller.h"
#include <QPrinterInfo>
#include <QPrintDialog>

QSharedPointer<Print>Print::m_print = QSharedPointer<Print>(nullptr);
Print::Print(QObject *parent) : QObject(parent)
{

    m_thread = new QThread;
    m_printer = new QPrinter(QPrinter::HighResolution);
    //m_pdfDir =  QCoreApplication::applicationDirPath()+"/curPdf.pdf";

    // = new QTextDocument();
    // m_threadDocument = new QTextDocument();
    m_process = new QProcess;
    m_printer->setPageSize(QPageSize(QPageSize::A4));
    m_printDialog = new QPrintDialog(m_printer,nullptr);
    m_printDialog->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::WindowMaximizeButtonHint|Qt::MSWindowsFixedSizeDialogHint|Qt::WindowCloseButtonHint);
   // m_printDialog->setModal(true);
    m_printDialog->hide();
    m_process->moveToThread(m_thread);
    // m_threadDocument->moveToThread(m_thread);
    this->moveToThread(m_thread);
    m_thread->start();
    connect(this,&Print::startSaveToPdf,this,[&](const QList<QString> &roleNameList, const QList<QVariant> &valueList,const QString&fileName)
    {
        saveDataToPdf(roleNameList, valueList,fileName);

    });




    connect(this,&Print::closeProcess,m_process,&QProcess::terminate);

    connect(this,&Print::printerDataSaveToPdf,this,[=](const QList<QString> &roleNameList, const QList<QVariant> &valueList,const QString&fileName,const QString &printName,int min,int max)
    {
        saveDataToPdf(roleNameList,valueList,fileName);
        Controller::instance()->delayMs(500);
        m_process->start(QString("lpr -P %1 -o page-ranges=%2-%3 -r %4").arg(printName).arg(min).arg(max).arg(fileName));


    });

    connect(this,&Print::printerDataAllToPdf,this,[=](const QList<QString> &roleNameList, const QList<QVariant> &valueList,const QString&fileName,const QString &printName){


        saveDataToPdf(roleNameList,valueList,fileName);
        Controller::instance()->delayMs(500);
        m_process->start(QString("lpr -P %1 -r %2").arg(printName).arg(fileName));

    });
    //    connect(this,&Print::startPrintFile,m_process,[=](const QString &printName,int min,int max, const QString&fileName)
    //    {

    //          // Controller::instance()->delayMs(5000);
    //           // m_process->start(QString("lpr -P %1 -o page-ranges=%2-%3 -r %4").arg(printName).arg(min).arg(max).arg(fileName));

    //    });

    //    connect(this,&Print::startPrintEntireFile,m_process,[=](const QString &printName,const QString&fileName)
    //    {


    //        //Controller::instance()->delayMs(5000);
    //            //m_process->start(QString("lpr -P %1 -r %2").arg(printName).arg(fileName));

    //    });


    connect(m_printDialog,QOverload<QPrinter*>::of (&QPrintDialog::accepted),this,[&](QPrinter*printer){

#ifdef Q_OS_WIN
        if(printer!=nullptr)
        {
            QTextDocument document;
            document.setHtml(createHtml(m_roleNameList,m_valueList,580,2000));
            printer->setOutputFormat(QPrinter::NativeFormat);
            document.print(printer);
        }

#elif defined Q_OS_LINUX
        if(printer!=nullptr)
        {

            printer->setOutputFormat(QPrinter::NativeFormat);
            QString pdfDir = QCoreApplication::applicationDirPath()+"/"+   QTime::currentTime().toString("HHmmsszzz")+".pdf";
            QString printName=printer->printerName();
            if(printer->printRange()!=QPrinter::AllPages)
            {


                int fromPage=printer->fromPage();
                int toPage = printer->toPage();
                if(fromPage!=0&&toPage!=0)
                {

                    emit printerDataSaveToPdf(m_roleNameList,m_valueList,pdfDir,printName,fromPage,toPage);
                    // emit startPrintFile(printName,fromPage,toPage,pdfDir);
                }

            }
            else
            {
                emit printerDataAllToPdf(m_roleNameList,m_valueList,pdfDir,printName);
                //emit startPrintEntireFile(printName,pdfDir);
            }

        }
#endif
        //m_document->end();
        // m_document->clear();
        m_roleNameList.clear();
        m_valueList.clear();


    });
}

Print *Print::instance()
{
    if(m_print.isNull())
    {

        m_print = QSharedPointer<Print>(new Print,&QObject::deleteLater);
    }
    return  m_print.data();
}

Print::~Print()
{
    m_thread->quit();
    m_thread->deleteLater();
    delete m_printer;
    // m_document->end();
    //m_document->clear();
    // m_document->deleteLater();
    // m_threadDocument->end();
    // m_threadDocument->clear();
    // m_threadDocument->deleteLater();
    m_roleNameList.clear();
    m_valueList.clear();
    emit closeProcess();
    m_process->deleteLater();
    delete m_printDialog;
}

void Print::saveToPdf(const QList<QString> &roleNameList, const QList<QVariant> &valueList,const QString&fileName)
{


    emit startSaveToPdf(roleNameList,valueList,fileName);

}

QString Print::createHtml(const QList<QString> &roleNameList, const QList<QVariant> &valueList,int tableSize,int size)
{
    int roleSize = roleNameList.size();
    int valueSize = valueList.size();
    if(valueSize/roleSize>=size)
    {
        valueSize=size*roleSize;
    }
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

QString Print::createAllHtml(const QList<QString> &roleNameList, const QList<QVariant> &valueList, int tableSize)
{
    return createHtml(roleNameList,valueList,tableSize,10000);
}

void Print::startPrint(const QList<QString> &roleNameList, const QList<QVariant> &valueList)
{
    m_roleNameList.clear();
    m_valueList.clear();
    m_roleNameList=roleNameList;
    m_valueList=valueList;
    QTextDocument document;
    document.setHtml(createHtml(roleNameList,valueList,580));
    m_printer->setOutputFormat(QPrinter::PdfFormat);
    m_printer->setFromTo(1,100);
    document.print(m_printer);
    m_printer->setPrintRange(QPrinter::AllPages);
    m_printer->setPageSizeMM(QSizeF(210,297));
    m_printer->setOutputFormat(QPrinter::NativeFormat);
    //    QSizeF s = QSizeF(m_printer->logicalDpiX()*2,m_printer->logicalDpiY()*2);
    //    m_document->setPageSize(s);

    m_printDialog->show();


}

void Print::printPreview(const QList<QString> &roleNameList, const QList<QVariant> &valueList)
{
    Q_UNUSED(roleNameList);
    Q_UNUSED(valueList);
    //    QPrintPreviewDialog printPreviewDialog ;
    //    printPreviewDialog.setWindowFlags(Qt::WindowStaysOnTopHint|Qt::WindowMaximizeButtonHint|Qt::MSWindowsFixedSizeDialogHint|Qt::WindowCloseButtonHint);
    //    printPreviewDialog.setMinimumSize(960,720);

    //    m_printer->setPageSize(QPageSize(QPageSize::A4));
    //    m_printer->setOutputFormat(QPrinter::PdfFormat);
    //    m_document->setHtml(createHtml(roleNameList,valueList,580));
    //    m_printer->setFromTo(0,10);
    //    QSizeF s = QSizeF(m_printer->logicalDpiX()/2,m_printer->logicalDpiY());
    //    m_document->setPageSize(s);
    //    m_printer->setPageSizeMM(QSizeF(210,297));
    //printPreviewDialog.done(100);

    //    connect(&printPreviewDialog,&QPrintPreviewDialog::customContextMenuRequested,this,[=](const QPoint &pos)
    //    {
    //        qDebug() << pos;
    //    });
    //    connect(&printPreviewDialog,&QPrintPreviewDialog::paintRequested,this,[&](QPrinter *printer)
    //    {
    //        m_document->print(printer);

    //    });
    //  printPreviewDialog.exec();

}

bool Print::saveDataToPdf(const QList<QString> &roleNameList, const QList<QVariant> &valueList, const QString &fileName)
{
    QTextDocument threadDocument;
    QString dataStr= createHtml(roleNameList,valueList,580,2000);
    threadDocument.setHtml(dataStr);

    QString curFileName;
    if(!fileName.endsWith(".pdf"))
    {
        curFileName= fileName+".pdf";
    }
    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    curFileName= Controller::instance()->fileNameFromQml(fileName);
    printer.setOutputFileName(curFileName);
    //#ifdef Q_OS_LINUX
    //    QSizeF s = QSizeF(printer.logicalDpiX()*1.1,printer.logicalDpiY());
    //    threadDocument.setPageSize(s);
    //#elif defined (Q_OS_WIN)
    //    QSizeF s = QSizeF(printer.logicalDpiX()*2,printer.logicalDpiY()*2);
    //    threadDocument.setPageSize(s);
    //#endif
    printer.setPageSizeMM(QSizeF(210,297));
    threadDocument.print(&printer);
    threadDocument.end();
    threadDocument.clear();
    //m_dataStr.clear();
    return true;
}

//void Print::saveFileToPdf(const QString &fileName)
//{

//}

//void Print::saveFileToPdf(QPrinter *printer, const QString &fileName)
//{
//    QString curFileName;
//    if(!fileName.endsWith(".pdf"))
//    {
//        curFileName= fileName+".pdf";
//    }
//    curFileName= Controller::instance()->fileNameFromQml(fileName);
//    if(printer!=nullptr)
//    {
//        printer->setOutputFormat(QPrinter::PdfFormat);


//        printer->setOutputFileName(curFileName);
//#ifdef Q_OS_LINUX
//        QSizeF s = QSizeF(printer->logicalDpiX()/2,printer->logicalDpiY());
//        m_threadDocument->setPageSize(s);
//#elif defined (Q_OS_WIN)
//        QSizeF s = QSizeF(printer->logicalDpiX()*2,printer->logicalDpiY()*2);
//        m_document->setPageSize(s);
//#endif
//        m_printer->setPageSizeMM(QSizeF(210,297));
//        m_threadDocument->print(printer);
//    }

//}
