/**
 * @file texture.h
 * @brief Defines the Texture type functions.
 * @if doc_files
 * @ingroup smug_graphics
 * @endif
 */

/**
 * @addtogroup smug_graphics
 * @{
 */

#ifndef SMUG_GRAPHICS_TEXTURE_H
#define SMUG_GRAPHICS_TEXTURE_H

#include <common.h>
#include <graphics/image.h>

struct _Texture;
typedef struct _Texture Texture;

/**
 * Creates a new texture from a loaded image. Requires OpenGL to be initialized.
 * @relates _Texture
 */
Texture* Texture_newFromImage(Image* image);

void Texture_activate(Texture* self);

/**
 * @relates _Texture
 */
void Texture_release(Texture* texture);

/**
 * @relates _Texture
 */
void Texture_delete(void* texture);

/**
 * @relates _Texture
 */
unsigned int Texture_getId(Texture* self);

/**
 * @relates _Texture
 */
unsigned int Texture_getWidth(Texture* self);

/**
 * @relates _Texture
 */
unsigned int Texture_getHeight(Texture* self);

float Texture_pixelToTextureCoordX(Texture* self, float pixel);
float Texture_pixelToTextureCoordY(Texture* self, float pixel);

#endif /* SMUG_GRAPHICS_TEXTURE_H */

/**@}*/
