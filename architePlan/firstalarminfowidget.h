#ifndef FIRSTALARMINFOWIDGET_H
#define FIRSTALARMINFOWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QFormLayout>
#include "graphicsWidget/graphicsitem.h"
#include <QDialog>
#include <QMouseEvent>
#include <QMovie>

class FirstAlarmInfoWidget : public QFrame
{
    Q_OBJECT
public:
    explicit FirstAlarmInfoWidget(QWidget *parent = nullptr);
    ~FirstAlarmInfoWidget();

    void retranslate();
    void updateGeometry();
    void showTitle();
    QString extLabelInfo();

public slots:
    void setFirstAlarmInfo(GraphicsItem *item);
    void setFirstAlarmInfo(const QString&extNum, const QString&loopNum,
                          const QString &addrNum, const QString &networkNum, const QString &timeStr, const QString &alarmType);
private:
   // QPushButton *m_firstFireBtn;
    QLabel *m_extLabel;
    QLabel *m_loopLabel;
    QLabel *m_addrNumLabel;
    QLabel *m_networkNumLabel;
    QLabel *m_locationLabel;
    QLabel *m_buildingNameLabel;
   // QLabel *m_deviceNumLabel;
    QLabel *m_equipmentModelLabel;
    QLabel *m_alarmTimeLabel;
    QLabel *m_floorLabel;
   // QLabel *m_operatorLabel;
    QLabel *m_sysLabel;
    QLabel *m_titleLabel;
    QLabel *m_fireGifLabel;
    QLabel *m_alarmGifLabel;
    QFormLayout *m_formLayout;
    QFormLayout *m_formLayout1;
    QFormLayout *m_formLayout2;
    QHBoxLayout *m_formHLayout;
    QMovie *m_fireMovie;
    QMovie *m_alarmMovie;

};

#endif // FIRSTFIREALARMINFOWIDGET_H
