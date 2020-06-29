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

    m_svgItem = new SvgItem();
    m_zoomIn=1.0;
    m_zoomOut=1.0;
    zoom(1.2);

    setRenderHint(QPainter::Antialiasing, true);
    setOptimizationFlags(QGraphicsView::DontSavePainterState);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);

    // setStyleSheet("background-color:black");
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
    if(scaleValue>1.0&&scaleValue<=4.0)
    {
        m_scale = scaleValue;
    }
    else
    {
        m_scale =1;
    }

}


void GraphicsView::wheelEvent(QWheelEvent *event)
{

    if (event->angleDelta().ry()>0)
    {
        zoomIn();
    }
    else
    {
        zoomOut();
    }
    event->accept();
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
            if(currentItem->currentState()!=tr("正常")&&currentItem->alarmType().endsWith(type))
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
    alarmStringList << tr("火警")<<tr("启动") << tr("监管") << tr("故障")<<tr("反馈") <<tr("屏蔽");
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

void GraphicsView::addGraphicsTextItem(const QPointF &pointF,const QString &alarmType)
{
    if(m_textItemHash.value(alarmType)==nullptr)
    {
        QGraphicsTextItem *textItem = new QGraphicsTextItem;
        //QGraphicsItemGroup *group = new QGraphicsItemGroup(item);
        textItem->setFlag(QGraphicsItem::ItemIgnoresTransformations);
        QFont font("Times New Roman",10);
        textItem->setFont(font);
        textItem->setDefaultTextColor(QColor(Qt::red));
        textItem->setPlainText(tr("首")+alarmType);
        m_scene->addItem(textItem);
        m_textItemHash[alarmType] = textItem;
        textItem->setPos(pointF);
    }

}

void GraphicsView::removeGraphicsTextItem(const QString &alarmType)
{

    QGraphicsTextItem *item= m_textItemHash.value(alarmType);
    if(item!=nullptr)
    {
        m_textItemHash.remove(alarmType);
        m_scene->removeItem(item);
        delete item;
        item = nullptr;
    }
    disconnect(this,&GraphicsView::currentScaleValue,nullptr,nullptr);
    // textItem->setPos(pointF);
}

void GraphicsView::clearGraphicsTextItem()
{
    foreach (QGraphicsTextItem*item, m_textItemHash.values())
    {
        if(item!=nullptr)
        {
            m_scene->removeItem(item);
            delete item;
            item = nullptr;
        }
    }
    m_textItemHash.clear();
    disconnect(this,&GraphicsView::currentScaleValue,nullptr,nullptr);
}

QGraphicsTextItem *GraphicsView::textItem(const QString &alarmType)
{
    return m_textItemHash.value(alarmType);
}

qreal GraphicsView::scaleValue()
{
    return  m_scale;
}

void GraphicsView::scaleValueChanged(qreal value)
{
    emit currentScaleValue(value);
}

void GraphicsView::updateSvg()
{
    m_svgItem->update();
}

QGraphicsScene *GraphicsView::sysScene()
{
    return m_sysViewScene;
}


void GraphicsView::loadPixmap(const QString &fileName)
{
    m_pixmapName = Controller::instance()->fileNameFromQml(fileName);
#ifdef Q_OS_LINUX
    if(!m_pixmapName.startsWith("/home"))
    {
        QFileInfo fileInfo(m_pixmapName);

        if(m_viewType==ArthitePlan)
        {
            m_pixmapName =QApplication::applicationDirPath()+"/楼层图/" +fileInfo.fileName();
        }
        else
        {
            m_pixmapName =QApplication::applicationDirPath()+"/系统图纸/" +fileInfo.fileName();
        }

    }
#endif
    m_svgItem->setSvgName(m_pixmapName);
}

void GraphicsView::zoomIn()
{
    qreal curScaleValue = transform().m11();
    if(curScaleValue<=4)
    {
        scale(m_scale,m_scale);
        // m_zoomIn=m_zoomIn*m_scale;
    }

    emit currentScaleValue(transform().m11());
}

void GraphicsView::zoomOut()
{
    qreal curScaleValue = transform().m11();
    if(curScaleValue>0.5)
    {
        scale(1.0/m_scale,1.0/m_scale);

    }
    emit currentScaleValue(transform().m11());

}
