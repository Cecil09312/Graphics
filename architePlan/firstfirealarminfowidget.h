#ifndef FIRSTFIREALARMINFOWIDGET_H
#define FIRSTFIREALARMINFOWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QFormLayout>
#include "graphicsWidget/graphicsitem.h"
#include <QDialog>
#include <QMouseEvent>

class FirstFireAlarmInfoWidget : public QDialog
{
    Q_OBJECT
public:
    explicit FirstFireAlarmInfoWidget(QWidget *parent = nullptr);
    ~FirstFireAlarmInfoWidget();
    void setFirstFireInfo(GraphicsItem *item);

signals:
   void toFirstFire();
public slots:
private:
   QPushButton *m_firstFireBtn;
    QLabel *m_extLabel;
    QLabel *m_loopLabel;
    QLabel *m_addrNumLabel;
    QLabel *m_networkNumLabel;
    QLabel *m_locationLabel;
    QLabel *m_buildingNameLabel;
    QLabel *m_deviceNumLabel;
    QLabel *m_equipmentModelLabel;
    QLabel *m_alarmTimeLabel;
    QLabel *m_floorLabel;
    QLabel *m_operatorLabel;
    QLabel *m_sysLabel;
};

#endif // FIRSTFIREALARMINFOWIDGET_H
