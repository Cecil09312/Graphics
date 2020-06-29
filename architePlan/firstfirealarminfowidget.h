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
    void retranslate();
    void updateGeometry();
    void setFirstFireInfo(const QString&extNum, const QString&loopNum,
                          const QString &addrNum, const QString &networkNum, const QString &timeStr);


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
    QLabel *m_titleLabel;
    QFormLayout *m_formLayout;
    QFormLayout *m_formLayout1;
    QFormLayout *m_formLayout2;
    QHBoxLayout *m_formHLayout;
};

#endif // FIRSTFIREALARMINFOWIDGET_H
