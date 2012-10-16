#include <stdlib.h>
#include <math.h>
#include <GL/glfw.h>

#include <common.h>
#include <utils/log.h>
#include <graphics/internal.h>

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
    DEBUG("Loading texture from image..."); Log_indent();

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

    int imgWidth = Image_getWidth(image);
    int imgHeight = Image_getHeight(image);
    /* OpenGL is a bitch for only allowing width and height that are powers of
     * two. It means we have to fill out the image to the nearest
     * such power before using it as a texture.
     */
    int neededWidth = _getClosestGreaterPowerOfTwo(imgWidth);
    int neededHeight = _getClosestGreaterPowerOfTwo(imgHeight);
    Image* imageForTexture = image;
    if (imgWidth != neededWidth ||
        imgHeight != neededHeight)
    {
        imageForTexture = Image_copy(image);
        Image_fillOut(imageForTexture, neededWidth, neededHeight);
        DEBUG("Had to rescale image for OpenGL texture. (%i x %i) -> (%i x %i)", imgWidth, imgHeight, Image_getWidth(imageForTexture), Image_getHeight(imageForTexture));
    }

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 Image_getWidth(imageForTexture),
                 Image_getHeight(imageForTexture),
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 Image_getData(imageForTexture));

    if (imgWidth != neededWidth ||
        imgHeight != neededHeight)
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
    tex->width = imgWidth;
    tex->height = imgHeight;
    tex->loaded = TRUE;

    Log_dedent(); DEBUG("...done.");
    return tex;
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
