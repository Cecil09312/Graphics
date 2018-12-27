#ifndef JSONEDIT_H
#define JSONEDIT_H

#include <QObject>
#include <QHash>
#include <QVariant>
#include <QMutex>
#include "control/controller.h"
#include "qmlforjson.h"
class JsonEdit : public QObject
{
    Q_OBJECT
public:

    static JsonEdit *instance();
    void insertRoot(const QVariant &root);
    void setRootValue(int parentIndex, const QString &name, const QVariant &value);
    void setRootName(int parentIndex, const QString &name);
    void setRootImage(int parentIndex, const QVariant &value);
    void insertChild(int parentIndex, const QVariant &child);
    void setChildName(int parentIndex, int childIndex,const QString &name);
    void setChildImage(int parentIndex, int childIndex, const QVariant &value);
    void setChildValue(int parentIndex,int childIndex,const QString&name,const QVariant &value);
    QVariant getChild(int parentIndex);
    QList<QVariant> getRoot() const;
    void writeFile(const QString &fileName);
    QVariant readFile(const QString &fileName);
    void fileClear();
public:
    class AutoDelete
    {
    public:
        ~AutoDelete()
        {
            if(m_jsonEdit!=nullptr)
            {
                delete m_jsonEdit;
                m_jsonEdit = nullptr;
            }
        }
    };

signals:

public slots:
private:
    explicit JsonEdit(QObject *parent = nullptr);


private:
    QList<QVariant>m_rootList;
    static JsonEdit *m_jsonEdit;
};

#endif // JSONVALUE_H
