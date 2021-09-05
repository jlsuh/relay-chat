#ifndef SHARED_UTILS_H_
#define SHARED_UTILS_H_

#include "buffer.h"
#include "package.h"
#include <commons/config.h>
#include <commons/string.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

void string_send(int socket, char* str);

#endif
