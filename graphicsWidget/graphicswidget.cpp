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
    m_svgItem = new QGraphicsSvgItem(":/images/background.svg");
   // m_graphicsView->setStyleSheet("border-image:url(:/images/background.svg)");
    hLayout->addWidget(m_graphicsView);
    m_graphicsView->setScene(m_scene);
    m_scene->addItem(m_svgItem);
    setLayout(hLayout);
}

GraphicsWidget::~GraphicsWidget()
{
    delete m_svgItem;
}

void GraphicsWidget::wheelEvent(QWheelEvent *event)
{
    qDebug() << event->modifiers();
}
