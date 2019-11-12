#include "globalgraphicsview.h"

GlobalGraphicsView::GlobalGraphicsView(QWidget *parent):
    QGraphicsView(parent)
{
    m_graphicsScene = new GlobalGraphicsScene(this);
    m_pixmapItem = new QGraphicsPixmapItem;
   //m_svgItem = new QGraphicsSvgItem;
    //m_graphicsScene->addItem(m_svgItem);
    m_graphicsScene->addItem(m_pixmapItem);
    setScene(m_graphicsScene);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    //QRectF currentRectF = m_graphicsScene->sceneRect();
   // fitInView(QRectF(0, 0, 1024, 768),Qt::KeepAspectRatio);
   fitInView(0,0,width(),height(),Qt::KeepAspectRatio);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,&GlobalGraphicsView::customContextMenuRequested,this,[=](const QPoint&/*pos*/)
    {
        if(m_graphicsScene!=nullptr)
        {
            m_graphicsScene->showMenu(QCursor::pos());
        }
    });
}

GlobalGraphicsView::~GlobalGraphicsView()
{

}

void GlobalGraphicsView::setPicture(const QString &pictureName)
{
    m_pixmapItem->setPixmap(QPixmap(pictureName));
//    if(!pictureName.endsWith(".svg"))
//    {
//        return;
//    }
//    QSvgRenderer *renderer = new QSvgRenderer(pictureName);
//    m_svgItem->setSharedRenderer(renderer);
//    m_pictureName = pictureName;
//    m_svgItem->update();
}

QString GlobalGraphicsView::pictureName()
{
    return m_pictureName;
}

GlobalGraphicsScene *GlobalGraphicsView::currentScene()
{
    return m_graphicsScene;
}
