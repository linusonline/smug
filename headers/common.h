/**
 * @file common.h
 * @brief Header file for common declarations
 * @if doc_files
 * @ingroup smug_common
 * @endif
 */

/**
 * @defgroup smug_common Common
 * @{
 */

#ifndef SMUG_COMMON_H
#define SMUG_COMMON_H

typedef unsigned char BOOL;
typedef double TIME;
#define TRUE  (1)
#define FALSE (0)
#ifndef NULL
    #define NULL (0)
#endif

#define smug_assert assert
#include <assert.h>

#include <stdlib.h>
#define allocate(type) ((type*)malloc(sizeof(type)))
#define allocatev(type, size) ((type*)malloc(sizeof(type) * size))

#define max(x, y) ((x) > (y) ? (x) : (y))
#define min(x, y) ((x) < (y) ? (x) : (y))

#define SMUG_NOOP() do {} while(0)

#ifndef SMUGEXPORT
    #define SMUGEXPORT
#endif /* SMUGEXPORT */

#endif /* SMUG_COMMON_H */

/**@}*/
