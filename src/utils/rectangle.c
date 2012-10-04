#include <common.h>

#include <utils/rectangle.h>

static Rectangle newRect;

Rectangle* Rectangle_new(void)
{
    Rectangle* newRect = allocate(Rectangle);
    newRect->x = 0.0;
    newRect->y = 0.0;
    newRect->w = 0.0;
    newRect->h = 0.0;
    return newRect;
}

void Rectangle_delete(Rectangle* self)
{
    free(self);
}

Rectangle Rectangle_create(float x, float y, float w, float h)
{
    newRect.x = x;
    newRect.y = y;
    newRect.w = w;
    newRect.h = h;
    return newRect;
}

// Vector Rectangle_getSize(Rectangle* self)
// {
// }

float Rectangle_getX(Rectangle* self)
{
    return self->x;
}

float Rectangle_getY(Rectangle* self)
{
    return self->y;
}

float Rectangle_getW(Rectangle* self)
{
    return self->w;
}

float Rectangle_getH(Rectangle* self)
{
    return self->h;
}

// void Rectangle_addVector(Rectangle* self, Vector v)
// {
// }

// Rectangle Rectangle_multiplyVector(Rectangle r, Vector s)
// {
// }
