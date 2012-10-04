/**
 * @file rectangle_type.h
 * @brief Defines a rectangle type.
 * @if doc_files
 * @ingroup smug_utils
 * @endif
 */

/**
 * @addtogroup smug_utils
 * @{
 */

#ifndef SMUG_UTILS_RECTANGLE_TYPE_H
#define SMUG_UTILS_RECTANGLE_TYPE_H

/**
 * A simple rectangle type.
 */
struct _Rectangle;
typedef struct _Rectangle Rectangle;

typedef struct _Rectangle
{
    float x;
    float y;
    float w;
    float h;
} _Rectangle;

#endif /* SMUG_UTILS_RECTANGLE_TYPE_H */
/**@}*/
