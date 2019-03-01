#include "globalgraphicsview.h"

GlobalGraphicsView::GlobalGraphicsView(QWidget *parent):
    QGraphicsView(parent)
{
    m_graphicsScene = new GlobalGraphicsScene(this);
    m_svgItem = new QGraphicsSvgItem;
    m_graphicsScene->addItem(m_svgItem);
    setScene(m_graphicsScene);
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
    QSvgRenderer *renderer = new QSvgRenderer(pictureName);
    m_svgItem->setSharedRenderer(renderer);
    m_pictureName = pictureName;
}

QString GlobalGraphicsView::pictureName()
{
    return m_pictureName;
}

GlobalGraphicsScene *GlobalGraphicsView::currentScene()
{
    return m_graphicsScene;
}
