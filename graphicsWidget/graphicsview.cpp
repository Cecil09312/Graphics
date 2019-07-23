#include "graphicsview.h"
#include <QDebug>
#include <QTransform>
#include <QScrollBar>
#include "control/controller.h"
#include "graphicsitem.h"
#include "architePlan/architeplanview.h"

qreal GraphicsView::m_scale =1;
GraphicsView::GraphicsView(QWidget *parent, int type):
    QGraphicsView(parent),
    m_viewType(type)
{

    m_svgItem = new QGraphicsSvgItem;
    zoom(1.2);
    setDragMode(QGraphicsView::ScrollHandDrag);
    //setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers),this));
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    //m_alarmStringList << "火警"<<"启动" << "监管" << "故障"<<"反馈" <<"屏蔽";
    if(m_viewType==ArthitePlan)
    {
       // setContextMenuPolicy(Qt::CustomContextMenu);
        m_scene = new GraphicsScene(this);
        setScene(m_scene);
        m_scene->addItem(m_svgItem);
        m_sysViewScene  = nullptr;
        connect(m_scene,&GraphicsScene::createItem,this,[=](GraphicsItem *item)
        {
            ArchitePlanView*architePlanView= Controller::instance()->getArchitePlanView();
            if(architePlanView!=nullptr)
            {
                QStandardItem *childItem= architePlanView->getItemFromView(this);
                if(item!=nullptr)
                {
                    if(childItem!=nullptr)
                    {
                        item->floorOfDevice() = childItem->text();
                        QStandardItem *parentItem = childItem->parent();
                        if(parentItem!=nullptr)
                        {
                            item->buildingName() = parentItem->text();
                        }
                    }

                }

            }
        });

    }
    else
    {
        m_scene = nullptr;
        m_sysViewScene = new QGraphicsScene(this);
        m_sysViewScene->addItem(m_svgItem);
        setScene(m_sysViewScene);
    }





}

GraphicsView::~GraphicsView()
{
    delete m_svgItem;
}

void GraphicsView::zoom(qreal scaleValue)
{
    if(scaleValue>1.0&&scaleValue<10.0)
    {
        m_scale = scaleValue;
    }
    else
    {
        m_scale =1;
    }

}


void GraphicsView::wheelEvent(QWheelEvent *e)
{
    if (e->angleDelta().ry()>0)
    {
        zoomIn();
    }
    else
    {
        zoomOut();
    }
    e->accept();
}


QPixmap GraphicsView::graphicsPixmap() const
{
    return QPixmap();
}

QString GraphicsView::pixmapName()
{
    return m_pixmapName;
}

QList<QGraphicsItem *>& GraphicsView::getItemList()
{

    return m_scene->getItemList();
}

QGraphicsItem *GraphicsView::getItem(int pos)
{
    QGraphicsItem *item = nullptr;
    if(m_scene!=nullptr)
    {
        item = m_scene->getItem(pos);
    }

    return item;
}

bool GraphicsView::haveAlarmType(const QString &type)
{
    QList<QGraphicsItem*> itemList= getItemList();
    bool haveAlarm = false;
    foreach (QGraphicsItem*item, itemList)
    {
        GraphicsItem*currentItem=  dynamic_cast<GraphicsItem*>(item);
        if(currentItem!=nullptr)
        {
            if(currentItem->currentState()==type)
            {
                haveAlarm = true;
                break;
            }
        }
    }
    return haveAlarm;
}

bool GraphicsView::haveAnyAlarm()
{
    bool isHave = false;
    QStringList alarmStringList;
    alarmStringList<< "火警"<<"启动" << "监管" << "故障"<<"反馈" <<"屏蔽";
    foreach (QString alarm, alarmStringList)
    {
        isHave =haveAlarmType(alarm);
        if(isHave)
        {
            break;
        }
    }
    return isHave;
}

QGraphicsScene *GraphicsView::currentGraphicsScene(int type)
{
    if(type==ArthitePlan)
    {
        return m_scene;
    }
    else
    {
        return m_sysViewScene;
    }
}

void GraphicsView::loadPixmap(const QString &fileName)
{
    m_pixmapName = Controller::instance()->fileNameFromQml(fileName);
    if(m_pixmapName.endsWith(".svg"))
    {
        QSvgRenderer *render = new QSvgRenderer(this);
        render->load(m_pixmapName);
        m_svgItem->setSharedRenderer(render);
    }

}

void GraphicsView::zoomIn()
{
    scale(m_scale,m_scale);
}

void GraphicsView::zoomOut()
{
    scale(1.0/m_scale,1.0/m_scale);
}
