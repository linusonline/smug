#include <string.h>

#include <common.h>
#include <utils/general.h>
#include <utils/log.h>
#include <utils/linkedlist.h>
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

typedef struct _SheetProxy
{
    char* fileName;
    char* dataFile;
    _SpriteSheet* sheet;
} _SheetProxy;

static LinkedList* sheets = NULL;

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

static _SpriteSheet* _new(const char* imageFile, const char* dataFile)
{
    int spriteWidth;
    int spriteHeight;
    if (*dataFile != NULL_CHAR)
    {
        if (!_parseDataFile(dataFile, &spriteWidth, &spriteHeight))
        {
            return NULL;
        }
        LOG(LOG_SPRITESHEET, "Successfully parsed Spritesheet data file.");
    }

    Image* spritesImage = Image_new();
    if (!Image_loadFromFile(spritesImage, imageFile))
    {
        Image_delete(spritesImage);
        ERROR("Could not create spritesheet due to failure to load image.");
        return NULL;
    }
    Texture* texture = Texture_newFromImage(spritesImage);
    Image_delete(spritesImage);
    spritesImage = NULL;
    if (texture == NULL)
    {
        ERROR("Could not create spritesheet due to failure to create texture.");
        return NULL;
    }

    LOG(LOG_SPRITESHEET, "Successfully loaded Spritesheet image file.");

    if (*dataFile == NULL_CHAR)
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

    LOG(LOG_SPRITESHEET, "Successfully created %i x %i (%i) Sprites for Spritesheet.", indexWidth, indexHeight, indexWidth * indexHeight);

    _SpriteSheet* newSheet = allocate(_SpriteSheet);
    newSheet->sprites = sprites;
    newSheet->nrSprites = indexWidth * indexHeight;
    newSheet->indexWidth = indexWidth;
    newSheet->texture = texture;
    LOG(LOG_SPRITESHEET, "Successfully created Spritesheet.");
    return newSheet;
}

static char* existingSheetImageFile = NULL;
static char* existingSheetDataFile = NULL;
static BOOL _identicalToExisting(void* sheet)
{
    return strcmp(((_SheetProxy*)sheet)->fileName, existingSheetImageFile) == 0 &&
           strcmp(((_SheetProxy*)sheet)->dataFile, existingSheetDataFile) == 0;
}

static _SheetProxy* _sheetExists(const char* imageFile, const char* dataFile)
{
    existingSheetImageFile = copyString(imageFile);
    existingSheetDataFile = copyString(dataFile);
    LinkedList* existing = LinkedList_getThose(sheets, _identicalToExisting);
    free(existingSheetImageFile);
    free(existingSheetDataFile);
    if (LinkedList_isEmpty(existing))
    {
        return NULL;
    }
    else
    {
        smug_assert(LinkedList_length(existing) == 1);
        _SheetProxy* s = (_SheetProxy*)LinkedList_getFirst(existing);
        LinkedList_delete(existing);
        return s;
    }
}

char* newZeroLengthString()
{
    char* c = allocate(char);
    *c = NULL_CHAR;
    return c;
}

_SheetProxy* SpriteSheet_newUnloaded(const char* imageFile, const char* dataFile)
{
    if (sheets == NULL)
    {
        sheets = LinkedList_new();
    }
    if (dataFile == NULL)
    {
        dataFile = newZeroLengthString();
    }

    _SheetProxy* sheet = _sheetExists(imageFile, dataFile);
    if (sheet != NULL)
    {
        WARNING("Tried to load the same SpriteSheet twice!");
        Log_indent();
        WARNING("Use SpriteSheet_isValid to see if the SpriteSheet is still valid,");
        WARNING("and SpriteSheet_reload to recreate it if it is not.");
        Log_dedent();
        return sheet;
    }

    _SheetProxy* data = allocate(_SheetProxy);
    data->fileName = copyString(imageFile);
    data->dataFile = copyString(dataFile);
    LinkedList_addLast(sheets, data);
    return data;
}

_SheetProxy* SpriteSheet_new(const char* imageFile, const char* dataFile)
{
    _SheetProxy* newProxy = SpriteSheet_newUnloaded(imageFile, dataFile);
    SpriteSheet_reload(newProxy);
    return newProxy;
}

BOOL SpriteSheet_isValid(_SheetProxy* self)
{
    return self->sheet != NULL;
}

void SpriteSheet_reload(_SheetProxy* self)
{
    smug_assert(!SpriteSheet_isValid(self));
    self->sheet = _new(self->fileName, self->dataFile);

    if (self->sheet == NULL)
    {
        WARNING("Could not load spritesheet from file %s with data in file %s", self->fileName, self->dataFile);
    }
}

Sprite* SpriteSheet_getSprite(_SheetProxy* self, int index)
{
    smug_assert(SpriteSheet_isValid(self));
    smug_assert(index < self->sheet->nrSprites);
    return self->sheet->sprites[index];
}

Sprite* SpriteSheet_getSpriteXY(_SheetProxy* self, int xIndex, int yIndex)
{
    smug_assert(xIndex < self->sheet->indexWidth);
    return SpriteSheet_getSprite(self, yIndex * self->sheet->indexWidth + xIndex);
}

int SpriteSheet_getNrOfSprites(_SheetProxy* self)
{
    return self->sheet->nrSprites;
}

void SpriteSheet_delete(_SheetProxy* self)
{
    for (int i = 0; i < self->sheet->nrSprites; i++)
    {
        Sprite_delete(self->sheet->sprites[i]);
    }
    Texture_delete(self->sheet->texture);
    free(self->sheet);
    self->sheet = NULL;
}
