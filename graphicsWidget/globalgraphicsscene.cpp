#include "globalgraphicsscene.h"
#include "control/controller.h"
#include "globalgraphicsitem.h"
#include <QDebug>
#include <QQmlContext>
#include "control/controller.h"
#include <QQuickItem>

GlobalGraphicsScene::GlobalGraphicsScene(QObject *parent):
    QGraphicsScene(parent),
    m_num(0)
{

    m_menu = new QMenu();
    m_removeItemAction = new QAction(tr("删除"),m_menu);
    m_editItemAction = new QAction(tr("编辑"),m_menu);
    m_removeSelectItemAction = new QAction(tr("删除选中"),m_menu);
    m_goToAchitePlanAction = new QAction(tr("转到建筑平面"),m_menu);
    m_clearItemAction = new QAction(tr("清空"),m_menu);
    //m_saveGeneralLayoutItemsAction = new QAction(tr("保存"),m_menu);

    m_globalItemSettingView = new QQuickView;
    m_globalItemSettingView->setSource(QUrl("qrc:/qml/itemSetting/GlobalItemSetting.qml"));
    m_globalItemSettingView->rootContext()->setContextProperty("GlobalItemSettingView",this);
    m_globalItemSettingView->setTitle(tr("建筑物信息设置界面"));
    m_globalItemObj = m_globalItemSettingView->rootObject();
    m_globalItemSettingView->setFlags(Qt::WindowStaysOnTopHint|Qt::WindowMaximizeButtonHint|Qt::MSWindowsFixedSizeDialogHint|Qt::WindowCloseButtonHint);
    m_menu->addAction(m_editItemAction);

    m_menu->addAction(m_removeItemAction);
    m_menu->addAction(m_removeSelectItemAction);
    m_menu->addAction(m_clearItemAction);
    m_menu->addAction(m_goToAchitePlanAction);


    connect(m_removeItemAction,&QAction::triggered,this,[=]()
    {
        int btnValue= QMessageBox::warning(nullptr,tr("删除提示窗口"),tr("删除当前建筑物以及其下的所有楼层和设备信息，确认要删除吗?"),QMessageBox::Ok,QMessageBox::No);
        if(btnValue==QMessageBox::Ok)
        {
            QGraphicsItem *graphicsItem =  itemAt(m_pointF,QTransform());
            GlobalGraphicsItem *item = dynamic_cast<GlobalGraphicsItem *>(graphicsItem);
            if(item!=nullptr)
            {
                removeItem(graphicsItem);
                m_globalCurrentItemList.removeOne(item);
                emit deleteGlobalItem(item);
                delete graphicsItem;
                graphicsItem = nullptr;
            }
            emit deleteItems();
        }

    });

    connect(m_removeSelectItemAction,&QAction::triggered,this,[=]()
    {

        int btnValue= QMessageBox::warning(nullptr,tr("删除提示窗口"),tr("删除所有选中的建筑物以及其下的所有楼层和设备信息，确认要删除吗?"),QMessageBox::Ok,QMessageBox::No);
        if(btnValue==QMessageBox::Ok)
        {
            QList<QGraphicsItem*>itemList =selectedItems();

            foreach (QGraphicsItem*currentItem, itemList)
            {
                GlobalGraphicsItem *item=  dynamic_cast<GlobalGraphicsItem *>(currentItem);
                if(item!=nullptr)
                {
                    if(!item->animalIsRunning())
                    {

                        removeItem(item);
                        m_globalCurrentItemList.removeOne(item);
                        emit deleteGlobalItem(item);
                        delete item;
                        item = nullptr;
                    }
                    else
                    {
                        QMessageBox::warning(nullptr,tr("信息警告"),tr("有报警信息存在，不能被删除"));
                    }
                }
            }

            emit deleteItems();
        }

    });

    // connect(m_saveGeneralLayoutItemsAction,&QAction::triggered,this,&GlobalGraphicsScene::saveGeneralLayoutItems);

    connect(m_editItemAction,&QAction::triggered,this,[=]()
    {
        // m_globalItemSettingView->close();
        m_globalItemSettingView->show();
        Q_ASSERT(m_globalItemObj);
        QMetaObject::invokeMethod(m_globalItemObj,"setBuileName",Q_ARG(QVariant,currentBuildName()));
        QMetaObject::invokeMethod(m_globalItemObj,"setGlobalItemValue",Q_ARG(QVariant,currentItemSize()));
        QMetaObject::invokeMethod(m_globalItemObj,"setGlobalIcon",Q_ARG(QVariant,currentItemIcon()));
        QMetaObject::invokeMethod(m_globalItemObj,"setPersonOnDuty",Q_ARG(QVariant,personOnDuty()));
    });
    connect(m_goToAchitePlanAction,&QAction::triggered,this,[=]()
    {
        QGraphicsItem *graphicsItem =  itemAt(m_pointF,QTransform());
        GlobalGraphicsItem *item = dynamic_cast<GlobalGraphicsItem *>(graphicsItem);
        if(item!=nullptr)
        {
            emit goToArchitePlan(item);
        }

    });
    connect(m_clearItemAction,&QAction::triggered,this,[=]()
    {

        int btnValue= QMessageBox::warning(nullptr,tr("清空提示窗口"),tr("清空所有建筑物以及其下的楼层和设备信息，确认要清空吗?"),QMessageBox::Ok,QMessageBox::No);
        if(btnValue==QMessageBox::Ok)
        {
            QList<QGraphicsItem*>itemList=  items();
            bool isHaveAlarm = false;
            foreach (QGraphicsItem*item, itemList)
            {
                GlobalGraphicsItem *globalGraphics = dynamic_cast<GlobalGraphicsItem*>(item);
                if(globalGraphics!=nullptr)
                {
                    if(globalGraphics->animalIsRunning())
                    {
                        isHaveAlarm = true;
                        break;
                    }
                }
            }
            if(!isHaveAlarm)
            {
                emit clearItem();
                m_globalCurrentItemList.clear();
            }
            else
            {
                QMessageBox::warning(nullptr,tr("信息警告"),tr("有报警信息存在，不能被清空"));
            }

        }

    });
    Q_ASSERT(m_globalItemObj);
    connect(m_globalItemObj,SIGNAL(setItemValue(qreal)),this,SLOT(setCurrentItemSize(qreal)));

}

GlobalGraphicsScene::~GlobalGraphicsScene()
{
    m_menu->close();
    delete m_menu;
    m_globalItemSettingView->close();
    m_globalItemSettingView->deleteLater();
}

void GlobalGraphicsScene::showMenu(const QPoint &point)
{
    QGraphicsItem *graphicsItem =  itemAt(m_pointF,QTransform());
    GlobalGraphicsItem *item = dynamic_cast<GlobalGraphicsItem *>(graphicsItem);

    UserManager::UserRight userRight=   Controller::instance()->getUserRight();
    if(userRight==UserManager::Super||userRight==UserManager::Administrator)
    {

        if(items().size()>1)
        {
            if(item!=nullptr)
            {
                if(ArchitePlanView::itemLimit())
                {
                    m_removeItemAction->setEnabled(true);
                }
                else
                {
                    m_removeItemAction->setEnabled(false);
                }

                m_editItemAction->setEnabled(true);
            }
            else
            {
                m_removeItemAction->setEnabled(false);
                m_editItemAction->setEnabled(false);
            }
            if(ArchitePlanView::itemLimit())
            {
                m_clearItemAction->setEnabled(true);
            }
            else
            {
                m_clearItemAction->setEnabled(false);
            }
            // m_saveGeneralLayoutItemsAction->setEnabled(true);

        }
        else
        {
            m_removeItemAction->setEnabled(false);
            m_clearItemAction->setEnabled(false);
            //m_saveGeneralLayoutItemsAction->setEnabled(false);
        }
        if(selectedItems().isEmpty())
        {
            m_removeSelectItemAction->setEnabled(false);
        }
        else
        {
            if(ArchitePlanView::itemLimit())
            {
                m_removeSelectItemAction->setEnabled(true);
            }
            else
            {
                m_removeSelectItemAction->setEnabled(false);
            }

        }

    }
    else
    {
        m_removeItemAction->setEnabled(false);
        m_editItemAction->setEnabled(false);
        m_removeSelectItemAction->setEnabled(false);
        m_clearItemAction->setEnabled(false);
        //m_saveGeneralLayoutItemsAction->setEnabled(false);
    }
    if(item!=nullptr)
    {
        m_goToAchitePlanAction->setEnabled(true);
    }
    else
    {
        m_goToAchitePlanAction->setEnabled(false);
    }
    m_menu->exec(point);
}

GlobalGraphicsItem * GlobalGraphicsScene::addGlobalGraphicsItem(QPointF point)
{
    GlobalGraphicsItem *item = new GlobalGraphicsItem(this);
    QList<QGraphicsItem*> curItemList= items();
    int curNum=0,num=0;
    if(curItemList.size()<=1)
    {
        m_num = 0;
    }
    foreach (QGraphicsItem*curItem, curItemList)
    {
        GlobalGraphicsItem *gItem = dynamic_cast<GlobalGraphicsItem *>(curItem);
        if(gItem!=nullptr)
        {
            QString buildName=  gItem->buildName();
            if(buildName.endsWith(tr("号楼")))
            {
                int len = QString(tr("号楼")).size();
                int size = buildName.size();
                curNum = buildName.left(size-len).toInt();
                num = qMax(num,curNum);
                m_num = num;
            }
        }
    }
    m_num++;

    item->setBuildName(QString(tr("%1号楼")).arg(m_num));
    item->setPos(point);
    addItem(item);

    m_globalCurrentItemList.push_back(item);
    return item;
}

void GlobalGraphicsScene::clearGraphicsItem()
{
    QList<QGraphicsItem*> graphicsItemList= items();
    foreach (QGraphicsItem*item, graphicsItemList)
    {
        GlobalGraphicsItem *currentItem = dynamic_cast<GlobalGraphicsItem *>(item);
        if(currentItem!=nullptr)
        {
            removeItem(currentItem);
            delete currentItem;
            currentItem = nullptr;
        }
    }
}

void GlobalGraphicsScene::closeQuickView()
{
    m_globalItemSettingView->close();
}



QString GlobalGraphicsScene::currentBuildName()
{
    QGraphicsItem *graphicsItem =  itemAt(m_pointF,QTransform());
    GlobalGraphicsItem *item = dynamic_cast<GlobalGraphicsItem *>(graphicsItem);
    if(item!=nullptr)
    {
        return item->buildName();
    }
    else
    {
        return "";
    }
}

void GlobalGraphicsScene::setCurrentBuildName(const QString &name)
{
    QGraphicsItem *graphicsItem =  itemAt(m_pointF,QTransform());
    GlobalGraphicsItem *item = dynamic_cast<GlobalGraphicsItem *>(graphicsItem);
    if(item!=nullptr)
    {
        item->setBuildName(name);
        emit setBuildingName(item,name);
    }
}

void GlobalGraphicsScene::setPersonOnDuty(const QString &personName)
{
    QGraphicsItem *graphicsItem =  itemAt(m_pointF,QTransform());
    GlobalGraphicsItem *item = dynamic_cast<GlobalGraphicsItem *>(graphicsItem);
    if(item!=nullptr)
    {
        item->setPersonOnDuty(personName);
        emit setCurPersonOnDuty(item,personName);
    }
}

qreal GlobalGraphicsScene::currentItemSize()
{
    QGraphicsItem *graphicsItem =  itemAt(m_pointF,QTransform());
    GlobalGraphicsItem *item = dynamic_cast<GlobalGraphicsItem *>(graphicsItem);
    if(item!=nullptr)
    {
        return item->itemSize();
    }
    else
    {
        return 0.0;
    }
}

void GlobalGraphicsScene::setCurrentItemSize(qreal size)
{
    QGraphicsItem *graphicsItem =  itemAt(m_pointF,QTransform());
    GlobalGraphicsItem *item = dynamic_cast<GlobalGraphicsItem *>(graphicsItem);
    if(item!=nullptr)
    {
        item->setItemSize(size);
        update();
        emit setItemSize(item,size);
    }
}

QString GlobalGraphicsScene::currentItemIcon()
{
    QGraphicsItem *graphicsItem =  itemAt(m_pointF,QTransform());
    GlobalGraphicsItem *item = dynamic_cast<GlobalGraphicsItem *>(graphicsItem);
    if(item!=nullptr)
    {
        return item->iconName();
    }
    else
    {
        return "";
    }
}

QString GlobalGraphicsScene::personOnDuty()
{
    QGraphicsItem *graphicsItem =  itemAt(m_pointF,QTransform());
    GlobalGraphicsItem *item = dynamic_cast<GlobalGraphicsItem *>(graphicsItem);
    if(item!=nullptr)
    {
        return item->personOnDuty();
    }
    else
    {
        return "";
    }
}

bool GlobalGraphicsScene::buildNameIsExist(const QString &name)
{
    bool isExist = false;
    QGraphicsItem *graphicsItem =  itemAt(m_pointF,QTransform());
    GlobalGraphicsItem *item = dynamic_cast<GlobalGraphicsItem *>(graphicsItem);
    QList <QGraphicsItem *>itemList=this->items();
    foreach (QGraphicsItem *curItem, itemList) {
        GlobalGraphicsItem *gItem = dynamic_cast<GlobalGraphicsItem *>(curItem);
        if(gItem!=nullptr)
        {
            if(item!=gItem&&gItem->buildName()==name)
            {
                isExist = true;
                break;
            }
        }
    }
    return isExist;
    //    if(item!=nullptr)
    //    {
    //        return item->buildName();
    //    }
}

GlobalGraphicsItem *GlobalGraphicsScene::itemFromBuildingName(const QString &buildingName)
{
    QList<QGraphicsItem*>itemList  = items();
    GlobalGraphicsItem*item = nullptr;
    foreach (QGraphicsItem*curItem, itemList)
    {
        GlobalGraphicsItem*gItem = dynamic_cast<GlobalGraphicsItem*>(curItem);
        if(gItem!=nullptr)
        {
            if(gItem->buildName()==buildingName)
            {
                item = gItem;
                break;
            }
        }
    }
    return item;
}

void GlobalGraphicsScene::retranslate()
{
    m_removeItemAction->setText(tr("删除"));
    m_editItemAction->setText(tr("编辑"));
    m_removeSelectItemAction->setText(tr("删除选中"));
    m_goToAchitePlanAction->setText(tr("转到建筑平面"));
    m_clearItemAction->setText(tr("清空"));
    m_globalItemSettingView->setTitle(tr("建筑物信息设置界面"));
    if(m_globalItemObj!=nullptr)
    {
        QMetaObject::invokeMethod(m_globalItemObj,"retranslate");
    }
}



QList<GlobalGraphicsItem *> &GlobalGraphicsScene::currentItemList()
{
    return  m_globalCurrentItemList;
}

void GlobalGraphicsScene::sortItemList(const QList<QString> &nameList)
{
    //QList<GlobalGraphicsItem *> tempList;
    if(nameList.size()==m_globalCurrentItemList.size())
    {
            foreach(GlobalGraphicsItem *item,m_globalCurrentItemList)
            {
                int key = nameList.indexOf(item->buildName());
                int itemIndex = m_globalCurrentItemList.indexOf(item);
                if(key!=itemIndex)
                {
                    m_globalCurrentItemList.swap(itemIndex,key);
                }
            }
    }


}

void GlobalGraphicsScene::setCurrentItemIcon(const QString &icon)
{
    QGraphicsItem *graphicsItem =  itemAt(m_pointF,QTransform());
    GlobalGraphicsItem *item = dynamic_cast<GlobalGraphicsItem *>(graphicsItem);
    if(item!=nullptr)
    {
        QString currentIconName=  Controller::instance()->fileNameFromQml(icon);
        item->setIconName(currentIconName);
        update();
        emit setItemIcon(item,currentIconName);
    }
}


void GlobalGraphicsScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        UserManager::UserRight userRight=  Controller::instance()->getUserRight();
        if(userRight==UserManager::Super|| userRight==UserManager::Administrator)
        {
            if(ArchitePlanView::itemLimit())
            {
                GlobalGraphicsItem*item=  addGlobalGraphicsItem(event->scenePos());
                emit addGlobalItem(item);
            }

        }
    }
}

void GlobalGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button()==Qt::RightButton)
    {
        m_pointF =event->scenePos();
    }
    QGraphicsScene::mousePressEvent(event);
}
