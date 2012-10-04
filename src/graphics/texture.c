#include <stdlib.h>
#include <math.h>

#include <common.h>
#include <utils/log.h>
#include <GL/glfw.h>
#include <graphics/internal.h>

#include <graphics/texture.h>
#include <graphics/texture_type.h>
#include <graphics/texture_internal.h>

static int _getClosestGreaterPowerOfTwo(int number)
{
    int n = 0;
    double power = ldexp(1.0, n);
    while (power < number)
    {
        ++n;
        power = ldexp(1.0, n);
    }
    return floor(power + 0.5);
}

static BOOL _invariant(Texture* self)
{
    smug_assert(self != NULL);
    return self != NULL &&
        ((self->loaded &&
          self->width > 0 &&
          self->height > 0 &&
          self->internalWidth >= self->width &&
          self->internalHeight >= self->height &&
          _getClosestGreaterPowerOfTwo(self->internalHeight) == self->internalHeight &&
          _getClosestGreaterPowerOfTwo(self->internalWidth) == self->internalWidth &&
          self->texid > 0) ||
         (!self->loaded &&
          self->width == 0 &&
          self->height == 0 &&
          self->internalWidth == 0 &&
          self->internalHeight == 0));
}

static Texture* loadTextureFromImage(Texture* tex, Image* image)
{
    smug_assert(image != NULL);
    smug_assert(image->data != NULL);
    DEBUG("Loading texture from image...");

    unsigned int texid;

    glGenTextures(1, &texid);
    glBindTexture(GL_TEXTURE_2D, texid);

#ifndef SMUG_GLES /* OpenGL ES 1.0 does not support glIsTexture */
    if (glIsTexture(texid) != GL_TRUE)
    {
        ERROR("Could not load texture from image.");
        return NULL;
    }
#endif

    /* OpenGL is a bitch for only allowing width and height that are powers of
     * two. It means we have to fill out the image to the nearest
     * such power before using it as a texture.
     */
    int neededWidth = _getClosestGreaterPowerOfTwo(image->width);
    int neededHeight = _getClosestGreaterPowerOfTwo(image->height);
    Image* imageForTexture = image;
    if (image->width != neededWidth ||
        image->height != neededHeight)
    {
        imageForTexture = Image_copy(image);
        Image_fillOut(imageForTexture, neededWidth, neededHeight);
        DEBUG("Had to rescale image for OpenGL texture. (%i x %i) -> (%i x %i)", image->width, image->height, imageForTexture->width, imageForTexture->height);
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageForTexture->width, imageForTexture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageForTexture->data);

    if (image->width != neededWidth ||
        image->height != neededHeight)
    {
        Image_delete(imageForTexture);
    }
#ifdef SMUG_GLES
    glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
#else
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
#endif

    glBindTexture(GL_TEXTURE_2D, 0);

    tex->texid = texid;
    tex->internalWidth = neededWidth;
    tex->internalHeight = neededHeight;
    tex->width = image->width;
    tex->height = image->height;
    tex->loaded = TRUE;

    return tex;
}

static BOOL loadEmptyTexture(Texture* tex, unsigned int width, unsigned height)
{
    unsigned int texid;

    glGenTextures(1, &texid);
    glBindTexture(GL_TEXTURE_2D, texid);

    int neededWidth = _getClosestGreaterPowerOfTwo(width);
    int neededHeight = _getClosestGreaterPowerOfTwo(height);
    glTexImage2D(GL_TEXTURE_2D, GL_RGBA, 4, neededWidth, neededHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if (glIsTexture(texid) != GL_TRUE)
    {
        ERROR("Could not load empty texture.");
        return FALSE;
    }
    printGLError();

    tex->texid = texid;
    tex->width = width;
    tex->height = height;
    tex->internalWidth = neededWidth;
    tex->internalHeight = neededHeight;
    tex->loaded = TRUE;

    return TRUE;
}

Texture* Texture_new(unsigned int width, unsigned int height)
{
    Texture* tex = allocate(Texture);
    if (loadEmptyTexture(tex, width, height))
    {
        smug_assert(_invariant(tex));
        return tex;
    }
    free(tex);
    return NULL;
}

Texture* Texture_newFromImage(Image* image)
{
    Texture* tex = allocate(Texture);
    if (loadTextureFromImage(tex, image))
    {
        smug_assert(_invariant(tex));
        return tex;
    }
    ERROR("Loading texture from image failed!");
    free(tex);
    return NULL;
}

void Texture_activate(Texture* self)
{
    smug_assert(glIsTexture(self->texid) == GL_TRUE);
    glBindTexture(GL_TEXTURE_2D, self->texid);
}

void Texture_release(Texture* self)
{
    smug_assert(_invariant(self));
	glDeleteTextures(1, &self->texid);
    printGLError();
	self->loaded = FALSE;
}

void Texture_delete(void* self)
{
    if (NULL != self)
    {
        smug_assert(_invariant(self));
        Texture* t = (Texture*)self;
        Texture_release(t);
        free(t);
    }
}

unsigned int Texture_getId(Texture* self)
{
    smug_assert(_invariant(self));
    return self->texid;
}

unsigned int Texture_getWidth(Texture* self)
{
    smug_assert(_invariant(self));
    return self->width;
}

unsigned int Texture_getHeight(Texture* self)
{
    smug_assert(_invariant(self));
    return self->height;
}

float Texture_pixelToTextureCoordX(Texture* self, float pixel)
{
    return (pixel / self->internalWidth);
}

float Texture_pixelToTextureCoordY(Texture* self, float pixel)
{
    return (pixel / self->internalHeight);
}
