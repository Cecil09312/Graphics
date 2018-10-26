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

void GraphicsWidget::loadPixmap(const QString &fileName)
{
    QString name = fileName;
    QList<QString> nameList=  name.split("file:///");
    int size = nameList.size();
    if(size>0)
        name = nameList.at(size-1);
   m_pixmapItem->setPixmap(QPixmap(name));
}


