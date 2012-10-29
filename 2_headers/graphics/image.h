/**
 * @file image.h
 * @brief Defines the Image type and related functions.
 * @if doc_files
 * @ingroup smug_graphics
 * @endif
 */

/**
 * @addtogroup smug_graphics
 * @{
 */

#ifndef SMUG_GRAPHICS_IMAGE_H
#define SMUG_GRAPHICS_IMAGE_H

#include <common.h>

struct _Image;
typedef struct _Image Image;

/**
 * Create a new empty image to pass to the load function.
 * @relates Image
 */
Image* Image_new(void);

/**
 * Create an image based on raw image data.
 * @param data The image data. Must be 4 bytes per pixel.
 * @param size Number of bytes needed to store the image data in memory.
 * @param width Width of the image in pixels.
 * @param height Height of the image in pixels.
 * @param alpha Use alpha data in image.
 * @relates Image
 */
Image* Image_newFromData(unsigned char* data, unsigned int size, unsigned int width, unsigned int height, BOOL alpha);

Image* Image_copy(Image* self);

void Image_fillOut(Image* image, int newWidth, int newHeight);

int Image_getWidth(Image* self);

int Image_getHeight(Image* self);

unsigned char* Image_getData(Image* self);

/**
 * Delete an image and its associated image data.
 * @param self The image to delete.
 * @relates Image
 */
void Image_delete(Image* self);

/**
 * Load an image from the specified file.
 *
 * The filename is interpreted in a platform-specific manner. Only PNG files are supported for now.
 *
 * @param filename The name of the image file to load.
 * @return TRUE on success, FALSE on failure.
 * @relates Image
 */
BOOL Image_loadFromFile(Image* self, const char* filename);

/**
 * Writes an image to a file.
 *
 * The filename is interpreted in a platform-specific manner. The file will be PNG-encoded.
 *
 * @param self The image to save.
 * @param filename The name of the file to save the image to.
 * @return TRUE on success, FALSE on failure.
 * @relates Image
 */
BOOL Image_saveToFile(Image* self, const char* filename);

#endif /* SMUG_GRAPHICS_IMAGE_H */

/**@}*/
