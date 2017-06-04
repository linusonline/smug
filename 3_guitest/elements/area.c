#include <stdio.h>

#include <map.h>
#include <area.h>
#include <platform/file.h>
#include <utils/sstring.h>

static const int MAX_LINE_LENGTH = 256;

typedef struct Area
{
    Map* spriteSheets;
    Map* tiles;
    GameObject** tileObjects;
} Area;

typedef struct TileProxy
{
    SpriteSheet* sheet;
    int spriteId;
} TileProxy;

static int strcmpVoid(void* str1, void* str1)
{
    return strcmp((char*)str1, (char*)str2);
}

static Area* _new()
{
    Area* newArea = allocate(Area);
    newArea->spriteSheets = Map_new(strcmpVoid);
    newArea->tiles = Map_new(strcmpVoid);
    newArea->tileObjects = NULL;
    return newArea;
}

void Area_delete(Area* self)
{
    free(self);
}

GameObject** createArea(const char* dataFile)
{
    Area* self = _new();

    FILE* file = NULL;
    file = fopen(dataFile,"rb");
    if (!file)
    {
        WARNING("Couldn't locate file '%s'.", fileName);
        Area_delete(self);
        return NULL;
    }
    while
    // char * fgets ( char * str, int num, FILE * stream );
    char lineBuffer[MAX_LINE_LENGTH];
    char* returnString = fgets(lineBuffer, MAX_LINE_LENGTH, file);
    if (ferror(file))
    {
        smug_assert(returnString == NULL);
        // Read error
    }
    if (feof(file))
    {
        // End-of-file
    }
    Stream* lineStream = Stream_new(lineBuffer);
    if (Stream_readToken(lineStream, "SHEET")
    {
        char* sheetName = Stream_readUntilWs(lineStream);
        char* imageFile = Stream_readString(lineStream);
        char* dataFile = Stream_readString(lineStream);
        SpriteSheet* sheet = SpriteSheet_newUnloaded(imageFile, dataFile);
        free(imageFile);
        free(dataFile);
        Map_set(self->spriteSheets, sheetName, sheet);
    }
    else if (Stream_readToken(lineStream, "TILE")
    {
        char* sheetName = Stream_readUntilWs(lineStream);
        int id = Stream_readInt(lineStream);
        char* tileName = Stream_readString(lineStream);
        TileProxy* tile = allocate(TileProxy);
        tile->sheet = (SpriteSheet*)Map_get(self->spriteSheets, sheetName);
        tile->spriteId = id;
        Map_set(self->tiles, tileName, tile);
    }
    else if (Stream_readToken(lineStream, "MAP")
    {
        int width = Stream_readInt(lineStream);
        int height = Stream_readInt(lineStream);
        int tileWidth = 0;
        int tileHeight = 0;
        if (!Stream_readToken("{"))
        {
            tileWidth = Stream_readInt(lineStream);
            tileHeight = Stream_readInt(lineStream);
            Stream_readToken("{");
        }
        tileObjects = allocatev(GameObject*, width * height);
        for (int x = 0; x < width; x++)
        {
            for (int y = 0; y < height; y++)
            {
                char* tileName = Stream_readString(lineStream);
                TileProxy* tile = (TileProxy*)Map_get(self->tiles, tileName);
                free(tileName);
                Sprite* sprite = SpriteSheet_getSprite(tile->sheet, tile->spriteId);
                if (tileWidth == 0 || tileHeight == 0)
                {
                    tileWidth = Sprite_getPixelWidth(sprite);
                    tileHeight = Sprite_getPixelHeight(sprite);
                }
                tileObjects[i] = newObjectNoData(OBJECT_BACKGROUND, x * tileWidth, y * tileHeight);
                Drawable* d = Drawable_newFromSpriteAndSize(sprite, tileWidth, tileHeight);
                GameObject_addDrawableAt(tileObjects[i], Drawable_newFromSpriteAndSize(s, tileWidth, tileHeight), 0.0f, 0.0f);
            }
        }
        Stream_readToken("}");
    }
    fclose(file);
}
