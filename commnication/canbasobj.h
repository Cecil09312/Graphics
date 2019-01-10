#ifndef CANBASOBJ_H
#define CANBASOBJ_H

#include <QObject>

class CanbasObj : public QObject
{
    Q_OBJECT
public:
    explicit CanbasObj(QObject *parent = nullptr);

signals:

public slots:
};

#endif // CANBASOBJ_H