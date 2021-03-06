/**
 * @file rectangle.h
 * @brief Defines the rectangle functions.
 * @if doc_files
 * @ingroup smug_utils
 * @endif
 */

/**
 * @addtogroup smug_utils
 * @{
 */

#ifndef UTILS_RECTANGLE_H
#define UTILS_RECTANGLE_H

struct _Rectangle;
typedef struct _Rectangle Rectangle;

typedef struct _Rectangle
{
    float x;
    float y;
    float w;
    float h;
} _Rectangle;

/**
 * Create a new Rectangle
 * @relates Rectangle
 */
Rectangle* Rectangle_new(void);

/**
 * Create a Rectangle from position and size
 * @relates Rectangle
 */
Rectangle Rectangle_create(float x, float y, float w, float h);

/**
 * Delete a Rectangle created with Rectangle_new
 * @relates Rectangle
 */
void Rectangle_delete(Rectangle* self);

/**
 * Get the size of the rectangle (as a diagonal vector).
 * @relates Rectangle
 */
// Vector Rectangle_getSize(Rectangle* self);

/**
 * Get the X axis position of the rectangle.
 * @relates Rectangle
 */
float Rectangle_getX(Rectangle* self);

/**
 * Get the Y axis position of the rectangle.
 * @relates Rectangle
 */
float Rectangle_getY(Rectangle* self);

/**
 * Get the width of the rectangle.
 * @relates Rectangle
 */
float Rectangle_getW(Rectangle* self);

/**
 * Get the height of the rectangle.
 * @relates Rectangle
 */
float Rectangle_getH(Rectangle* self);

/**
 * Add a vector to the position of the rectangle.
 * @relates Rectangle
 */
// void Rectangle_addVector(Rectangle* self, Vector v);

/**
 * Multiply the rectangles coordinates and size by a vector.
 * Used for scaling a rectangle.
 * @relates Rectangle
 */
// Rectangle Rectangle_multiplyVector(Rectangle self, Vector s);

#endif /* UTILS_RECTANGLE_H */

/**@}*/
