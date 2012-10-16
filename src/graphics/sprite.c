#include <common.h>
#include <utils/log.h>
#include <utils/rectangle.h>
#include <graphics/texture.h>
#include <graphics/renderbatch.h>

#include <graphics/sprite.h>
#include <graphics/sprite_internal.h>

static Sprite* Sprite_newFromPixelCoordsRect(Texture* texture, Rectangle rect)
{
    smug_assert(texture != NULL);
    Sprite* newSprite = allocate(Sprite);
    newSprite->texture = texture;
    newSprite->rect = rect;
    return newSprite;
}

Sprite* Sprite_newEmpty()
{
    Sprite* newSprite = allocate(Sprite);
    newSprite->texture = NULL;
    newSprite->rect = Rectangle_create(0, 0, 0, 0);
    return newSprite;
}

Sprite* Sprite_newFromPixelCoords(Texture* texture, int x, int y, int w, int h)
{
    return Sprite_newFromPixelCoordsRect(texture, Rectangle_create(x, y, w, h));
}

void Sprite_addRenderData(Sprite* self, RenderBatch* renderBatch, float posX, float posY, float width, float height)
{
    if (self->texture == NULL)
    {
        WARNING("Sprite texture was null.");
        return;
    }
    RenderBatch_addTexturedRect(renderBatch,
                                posX, posY, posX + width, posY + height,
                                Texture_pixelToTextureCoordX(self->texture, self->rect.x),
                                Texture_pixelToTextureCoordY(self->texture, self->rect.y),
                                Texture_pixelToTextureCoordX(self->texture, self->rect.x + self->rect.w),
                                Texture_pixelToTextureCoordY(self->texture, self->rect.y + self->rect.h));
}

Texture* Sprite_getTexture(Sprite* self)
{
    smug_assert(self != NULL);
    return self->texture;
}

unsigned int Sprite_getTextureId(Sprite* self)
{
    smug_assert(self != NULL);
    return self->texture == NULL ? 0 : Texture_getId(self->texture);
}

void Sprite_delete(void* sprite)
{
    if (NULL != sprite)
    {
        free(sprite);
    }
}
