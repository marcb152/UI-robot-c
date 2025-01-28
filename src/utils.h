#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#ifdef NDEBUG // release mode
#define TRACE(fmt, ...)

#else // debug mode
/// @warning : use gcc idiom for variable arguments (works even if no variable
/// argument are added)
#define TRACE(fmt, ...)                                                        \
  do {                                                                         \
    fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, __LINE__, __func__,          \
            ##__VA_ARGS__);                                                    \
    fflush(stderr);                                                            \
  } while (0);
#endif // NDEBUG

#endif // UTILS_H
