#ifndef __EXECUTER_H__
#define __EXECUTER_H__

#include <sys/types.h> /* clockid_t */
#include <stddef.h>  /* size_t */


typedef PeriodicExecutor PeriodicExecutor;

PeriodicExecutor *PeriodicExecutor_Create(const char *_name, clockid_t _clk_id);

int PeriodicExecutor_Add(PeriodicExecutor *_executor, int (*_taskFunction)(void *), void *_context, size_t _period_ms);

size_t PeriodicExecutor_Run(PeriodicExecutor *_executor);

size_t PeriodicExecutor_Pause(PeriodicExecutor *_executor);

void PeriodicExecutor_Destroy(PeriodicExecutor *_executor);

#endif /* __EXECUTER_H__ */