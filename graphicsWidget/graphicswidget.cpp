#include "GraphicsWidget.h"
#include <QHBoxLayout>
#include <QDebug>
#include  <QGraphicsRectItem>
#include  <QRect>
#include <QPen>
#include <QHBoxLayout>
#include <QUrl>
#include <QDir>

GraphicsWidget::GraphicsWidget(QWidget *parent)
    : QWidget(parent)
{

    QHBoxLayout *hLayout = new QHBoxLayout;
    m_graphicsView = new GraphicsView(this);
    m_scene = new GraphicsScene(m_graphicsView);
    m_pixmapItem = new QGraphicsPixmapItem;
    hLayout->addWidget(m_graphicsView);
    m_graphicsView->setScene(m_scene);
    //m_scene->addItem(m_svgItem);
    m_scene->addItem(m_pixmapItem);
    setLayout(hLayout);

}

GraphicsWidget::~GraphicsWidget()
{
    //delete m_svgItem;
    delete m_pixmapItem ;
}

QPixmap GraphicsWidget::graphicsPixmap() const
{
    return m_pixmapItem->pixmap();
}

QString GraphicsWidget::pixmapName()
{
   return m_pixmapName;
}

void GraphicsWidget::loadPixmap(const QString &fileName)
{
    m_pixmapName = fileName;
    if(m_pixmapName.startsWith("file:///"))
    {
        QList<QString> nameList=  m_pixmapName.split("file:///");
        int size = nameList.size();
        if(size>0)
        {
            m_pixmapName = nameList.at(size-1);
        }
    }

   m_pixmapItem->setPixmap(QPixmap(m_pixmapName));

}


