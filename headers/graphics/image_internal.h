#ifndef SMUG_GRAPHICS_IMAGE_INTERNAL_H
#define SMUG_GRAPHICS_IMAGE_INTERNAL_H

/**
 * Holds image data, file name and meta data of an image.
 */
typedef struct _Image
{
    unsigned char* data;
    unsigned char* file;
    unsigned int size;
    unsigned int width;
    unsigned int height;
    unsigned int bpp;
    int channels;
} _Image;

#endif /* SMUG_GRAPHICS_IMAGE_INTERNAL_H */
