#include "segfault.h"
jmp_buf sigEnv ;
void segFaultReceive(int sig)
{

#ifdef Q_OS_LINUX
    siglongjmp(sigEnv,1);
#endif

}
