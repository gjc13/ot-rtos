#ifndef COMMON_LOG_H_
#define COMMON_LOG_H_

#include <stdio.h>

#define log(format, ...) fprintf(stderr, format, ##__VA_ARGS__)

#endif

