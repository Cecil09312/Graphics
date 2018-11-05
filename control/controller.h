#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <QMutex>
#include <QDebug>

template<class T>
class Controller
{
public:
    static T *instance()
    {
        return &m_t;
    }

protected:
    Controller(){}
    Controller(const Controller&){}
    Controller &operator=(const Controller&){}
private:
   static T m_t;

};
#define DECL_SINGLETON(T) friend class Controller<T>
#endif // CONTROLLER_H
template<class T>__declspec(selectany) T Controller<T>:: m_t ;



