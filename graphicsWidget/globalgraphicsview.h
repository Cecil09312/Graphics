#ifndef GLOBALGRAPHICSVIEW_H
#define GLOBALGRAPHICSVIEW_H
#include <QGraphicsView>
#include "globalgraphicsitem.h"
#include "globalgraphicsscene.h"
#include <QSvgRenderer>
#include "svgitem.h"

class GlobalGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GlobalGraphicsView(QWidget *parent = Q_NULLPTR);
    ~GlobalGraphicsView();
    void setPicture(const QString &pictureName);
    QString pictureName();
    GlobalGraphicsScene *currentScene();
    void retranslate();

private:
    SvgItem *m_item;
    GlobalGraphicsScene *m_graphicsScene;
    QString m_pictureName;
};

#endif // GLOBALGRAPHICSVIEW_H
