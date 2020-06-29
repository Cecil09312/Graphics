#ifndef SEGFAULT_H
#define SEGFAULT_H

#include <signal.h>
#include <setjmp.h>
# include <qsystemdetection.h>
#ifdef __cplusplus

extern "C"
{
#endif
extern jmp_buf sigEnv;
extern void segFaultReceive(int sig);
#ifdef __cplusplus
}
#endif
#endif // SEGFAULT_H
