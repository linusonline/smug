#include <common.h>
#include <utils/log.h>
#include <graphics/sprite.h>
#include <graphics/texture.h>
#include <platform/file.h>

#include <graphics/spritesheet.h>

typedef struct _SpriteSheet
{
    Sprite** sprites;
    int indexWidth;
    int nrSprites;
    Texture* texture;
} _SpriteSheet;

static BOOL _parseDataFile(const char* fileName, int* width, int* height)
{
     File* file = NULL;
    file = File_fopen(fileName,"rb");
    if (!file)
    {
        WARNING("Couldn't locate file '%s'.", fileName);
        return FALSE;
    }
    int w;
    int h;
    int ret = File_fscanf(file, "GRID %i %i", &w, &h);
    File_fclose(file);
    if (ret < 2)
    {
        ERROR("Error while parsing spritesheet data file!");
        return FALSE;
    }
    *width = w;
    *height = h;
    return TRUE;
}

SpriteSheet* SpriteSheet_new(const char* imageFile, const char* dataFile)
{
    int spriteWidth;
    int spriteHeight;
    if (dataFile != NULL)
    {
        if (!_parseDataFile(dataFile, &spriteWidth, &spriteHeight))
        {
            return NULL;
        }
        DEBUG("Successfully parsed Spritesheet data file.");
    }

    Image* spritesImage = Image_new();
    Image_loadFromFile(spritesImage, imageFile);
    Texture* texture = Texture_newFromImage(spritesImage);
    Image_delete(spritesImage);
    spritesImage = NULL;

    DEBUG("Successfully loaded Spritesheet image file.");

    if (dataFile == NULL)
    {
        spriteWidth = Texture_getWidth(texture);
        spriteHeight = Texture_getHeight(texture);
    }

    int indexWidth = Texture_getWidth(texture) / spriteWidth;
    int indexHeight = Texture_getHeight(texture) / spriteHeight;
    if (indexWidth * indexHeight > 1024)
    {
        // TODO: Add better sanity checks, like on texture size.
        ERROR("Spritesheet too large.");
        Texture_delete(texture);
        return NULL;
    }
    Sprite** sprites = allocatev(Sprite*, indexWidth * indexHeight);
    for (int y = 0; y < indexHeight; y++)
    {
        for (int x = 0; x < indexWidth; x++)
        {
            sprites[y * indexWidth + x] = Sprite_newFromPixelCoords(texture, x * spriteWidth, y * spriteHeight, spriteWidth, spriteHeight);
        }
    }

    DEBUG("Successfully created Sprites for Spritesheet.");

    SpriteSheet* newSheet = allocate(SpriteSheet);
    newSheet->sprites = sprites;
    newSheet->nrSprites = indexWidth * indexHeight;
    newSheet->indexWidth = indexWidth;
    newSheet->texture = texture;
    DEBUG("Successfully created Spritesheet.");
    return newSheet;
}

Sprite* SpriteSheet_getSprite(SpriteSheet* self, int index)
{
    smug_assert(index < self->nrSprites);
    return self->sprites[index];
}

Sprite* SpriteSheet_getSpriteXY(SpriteSheet* self, int xIndex, int yIndex)
{
    smug_assert(xIndex < self->indexWidth);
    return SpriteSheet_getSprite(self, yIndex * self->indexWidth + xIndex);
}

int SpriteSheet_getNrOfSprites(SpriteSheet* self)
{
    return self->nrSprites;
}

void SpriteSheet_delete(SpriteSheet* self)
{
    for (int i = 0; i < self->nrSprites; i++)
    {
        Sprite_delete(self->sprites[i]);
    }
    Texture_delete(self->texture);
    free(self);
}
