#pragma once
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"

#ifndef COLOR_LOG
#define COLOR_LOG 1
#endif

#ifndef MINIMAL_LOG
#define MINIMAL_LOG 0
#endif

#if MINIMAL_LOG
#define DEBUG putchar('\n');printf
#define INFO putchar('\n');printf
#define NOTICE putchar('\n');printf
#define ALERT putchar('\n');printf
#define WARN putchar('\n');printf
#define ERROR putchar('\n');printf
#else

#define clean_errno() (errno == 0 ? "None" : strerror(errno))
#define MILLIS() to_ms_since_boot(get_absolute_time())
extern const char *getName(); // replacement for Task::getName when logging outside of tasks
extern int doDebug;


#ifdef BUILD_TYPE_DEBUG
#if COLOR_LOG
#define INFO(M, ...) printf("\033[37;42m%12lu [INFO]   %-16s (%s:%s@%d) " M "\033[39m\033[49m\n", MILLIS(), getName(), strrchr(__FILE__,'/')+1, __func__, __LINE__, ##__VA_ARGS__)
#else
#define INFO(M, ...) printf("%12lu [INFO]   %-16s (%s:%s@%d) " M "\n", MILLIS(), getName(), strrchr(__FILE__,'/')+1, __func__, __LINE__, ##__VA_ARGS__)
#endif //COLOR_LOG
#define DEBUG(M, ...) {doDebug && printf("%12lu [DEBUG]  %-16s (%s:%s@%d) " M "\n", MILLIS(), getName(), strrchr(__FILE__,'/')+1, __func__, __LINE__, ##__VA_ARGS__);}
#else
#define DEBUG(msg,...)
#define INFO(msg,...)
#endif

#if COLOR_LOG
#define NOTICE(M, ...) printf("\033[37;43m%12lu [NOTICE] %-16s (%s:%s@%d) " M "\033[39m\033[49m\n", MILLIS(), getName(), strrchr(__FILE__,'/')+1, __func__, __LINE__, ##__VA_ARGS__)
#define ALERT(M, ...) printf("\033[37;41m%12lu [ALERT]  %-16s (%s:%s@%d) " M "\033[39m\033[49m\n", MILLIS(), getName(), strrchr(__FILE__,'/')+1, __func__, __LINE__, ##__VA_ARGS__)
#define WARN(M, ...) printf("\033[37;43m%12lu [WARN]   %-16s (%s:%s@%d: errno=>%s) " M "\033[39m\033[49m\n", MILLIS(), getName(), strrchr(__FILE__,'/')+1, __func__, __LINE__, clean_errno(), ##__VA_ARGS__)
#define ERROR(M, ...) printf("\033[37;41m%12lu [ERROR]  %-16s (%s:%s@%d: errno=>%s) " M "\033[39m\033[49m\n", MILLIS(), getName(), strrchr(__FILE__,'/')+1, __func__, __LINE__, clean_errno(), ##__VA_ARGS__)
#else //!COLOR_LOG
#define NOTICE(M, ...) printf("%12lu [NOTICE] %-16s (%s:%s@%d) " M "\n", MILLIS(), getName(), strrchr(__FILE__,'/')+1, __func__, __LINE__, ##__VA_ARGS__)
#define ALERT(M, ...) printf("%12lu [ALERT]  %-16s (%s:%s@%d) " M "\n", MILLIS(), getName(), strrchr(__FILE__,'/')+1, __func__, __LINE__, ##__VA_ARGS__)
#define WARN(M, ...) printf("%12lu [WARN]   %-16s (%s:%s@%d: errno=>%s) " M "\n", MILLIS(), getName(), strrchr(__FILE__,'/')+1, __func__, __LINE__, clean_errno(), ##__VA_ARGS__)
#define ERROR(M, ...) printf("%12lu [ERROR]  %-16s (%s:%s@%d: errno=>%s) " M "\n", MILLIS(), getName(), strrchr(__FILE__,'/')+1, __func__, __LINE__, clean_errno(), ##__VA_ARGS__)
#endif //!COLOR_LOG

#endif //!MINIMAL_LOG

extern void hexDump(const char *leader, const void* data, size_t size);

#define DECLARE_STUB(proto, body) proto { NOTICE(""); body;}


