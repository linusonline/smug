#include <stdlib.h>
#include <string.h>

#include <common.h>
#include <utils/log.h>
#include <platform/file.h>
#include <graphics/lodepng/lodepng.h>

#include <graphics/image.h>
#include <graphics/image_internal.h>

static BOOL _invariant(Image* self)
{
    return
        self != NULL &&
        self->width * self->height * self->bpp == self->size &&
        self->width > 0 &&
        self->height > 0 &&
        self->bpp == 4 &&
        (self->channels == 4 || self->channels == 3);
}

Image* Image_new()
{
    Image* newImage = allocate(Image);
    newImage->data = NULL;
    newImage->file = NULL;
    newImage->size = 0;
    newImage->width = 0;
    newImage->height = 0;
    newImage->channels = 0;
    newImage->bpp = 4;

    return newImage;
}

Image* Image_newFromData(unsigned char* data, unsigned int size, unsigned int width, unsigned int height, BOOL alpha)
{
    smug_assert(size == width * height * 4); // Only support 32 bit images right now.
    Image* newImage = allocate(Image);
    newImage->size = size;
    newImage->channels = alpha ? 4 : 3;
    newImage->width = width;
    newImage->height = height;
    newImage->bpp = 4;
    newImage->data = allocatev(unsigned char, newImage->size);
    memcpy(newImage->data, data, newImage->size);
    newImage->file = NULL;
    smug_assert(_invariant(newImage));
    // TODO: Set all alpha bytes to 255 in image data.
    DEBUG("New image. Width: %i, height: %i, alpha: %s, size %i", newImage->width, newImage->height, alpha ? "TRUE" : "FALSE", newImage->size);
    return newImage;
}

Image* Image_copy(Image* self)
{
    smug_assert(_invariant(self));
    Image* copy = Image_new();
    copy->file = self->file;
    copy->size = self->size;
    copy->width = self->width;
    copy->height = self->height;
    copy->channels = self->channels;

    copy->data = allocatev(unsigned char, copy->size);
    memcpy(copy->data, self->data, copy->size);
    smug_assert(_invariant(copy));
    return copy;
}

void Image_fillOut(Image* self, int newWidth, int newHeight)
{
    smug_assert(_invariant(self));
    smug_assert(newWidth >= self->width && newHeight >= self->height);
    int newSize = newWidth * newHeight * self->bpp;
    unsigned char* newData = allocatev(unsigned char, newSize);
    memset(newData, 0, newSize);

    int oldByteWidth = self->width * self->bpp;
    int newByteWidth = newWidth * self->bpp;
    for (int y = 0; y < self->height; y++)
    {
        memcpy(newData + newByteWidth * y, self->data + oldByteWidth * y, oldByteWidth);
    }
    free(self->data);
    self->data = newData;
    self->width = newWidth;
    self->height = newHeight;
    self->size = newWidth * newHeight * self->bpp;
    smug_assert(_invariant(self));
}

int Image_getWidth(Image* self)
{
    smug_assert(_invariant(self));
    return self->width;
}

int Image_getHeight(Image* self)
{
    smug_assert(_invariant(self));
    return self->height;
}

unsigned char* Image_getData(Image* self)
{
    smug_assert(_invariant(self));
    return self->data;
}

void Image_delete(Image* self)
{
    if (self->data)
    {
        free(self->data);
        self->data = NULL;
    }
    if (self->file)
    {
        free(self->file);
        self->file = NULL;
    }

    free(self);
}

static unsigned char* _loadFile(const char* filename, unsigned int* buffersize)
{
 	File* file = File_fopen(filename,"rb");
	if (!file)
	{
		WARNING("Couldn't locate file '%s'.", filename);
		return 0;
	}
    DEBUG("Successfully opened file '%s'.", filename);

	File_fseek(file, 0, SMUG_SEEK_END);
	int filelen = File_ftell(file);
	File_fseek(file, 0, SMUG_SEEK_SET);

    DEBUG("File length: '%i'.", filelen);

	unsigned char* buffer = (unsigned char*)malloc(filelen);
	if (File_fread(file, buffer, 1, filelen) != filelen)
	{
		ERROR("Did not read correct amount of bytes.");
		File_fclose(file);
		free(buffer);
        return 0;
	}

    File_fclose(file);
    *buffersize = filelen;
    return buffer;
}

static BOOL _saveFile(const char* filename, unsigned char* buffer, unsigned int buffersize)
{
 	FILE* file = fopen(filename,"w+b");
	if (!file)
	{
		WARNING("Couldn't locate or create file '%s'.", filename);
		return FALSE;
	}
    DEBUG("Successfully opened file '%s'.", filename);


    DEBUG("Buffer length: '%i'.", buffersize);

	if (fwrite(buffer, 1, buffersize, file) != buffersize)
	{
		ERROR("Did not write correct amount of bytes.");
		fclose(file);
        return FALSE;
	}

    fclose(file);

    return TRUE;
}

static BOOL _decodePNG(Image* image, unsigned char* buffer, unsigned int buffersize)
{
    LodePNG_Decoder decoder;
    LodePNG_Decoder_init(&decoder);
    LodePNG_decode(&decoder, &image->data, &image->size, buffer, buffersize);

    if(decoder.error)
    {
        ERROR("PNG decoding failed, error: %d", decoder.error);
        return FALSE;
    }

    image->width = decoder.infoPng.width;
    image->height = decoder.infoPng.height;
    image->channels = LodePNG_InfoColor_getChannels(&decoder.infoPng.color);

    DEBUG("Loaded PNG. Size %i x %i, %i channels. Size %i", image->width, image->height, image->channels, image->size);

    LodePNG_Decoder_cleanup(&decoder);

    return TRUE;
}

static BOOL _encodePNG(unsigned char** buffer, unsigned int* buffersize, Image* image)
{
    LodePNG_Encoder encoder;
    LodePNG_Encoder_init(&encoder);
    LodePNG_encode(&encoder, buffer, buffersize, image->data, image->width, image->height);

    if(encoder.error)
    {
        ERROR("PNG encoding failed, error: %d", encoder.error);
        return FALSE;
    }

    LodePNG_Encoder_cleanup(&encoder);

    return TRUE;
}

BOOL Image_loadFromFile(Image* self, const char* filename)
{
    unsigned char* buffer = NULL;

    DEBUG("Loading image from file '%s'", filename);

    unsigned int buffersize;
    buffer = _loadFile(filename, &buffersize);
    if (NULL == buffer)
    {
        WARNING("Could not open image '%s'", filename);
        return FALSE;
    }

    // Decode according to file ending
    int len = strlen(filename);
    BOOL retval = FALSE;
    if (0 == strncmp(&filename[len-4], ".png", 3))
    {
        DEBUG("Decoding image as PNG");
        retval = _decodePNG(self, buffer, buffersize);
    }
    else
    {
        DEBUG("No known file format, decoding image as PNG");
        retval = _decodePNG(self, buffer, buffersize);
    }

    if (!retval)
    {
        ERROR("Could not load image");
        return FALSE;
    }

    DEBUG("Successfully loaded image");
    smug_assert(_invariant(self));
    return TRUE;
}

BOOL Image_saveToFile(Image* self, const char* filename)
{
    unsigned char* buffer = NULL;
    unsigned int buffersize;
    DEBUG("Saving image to file '%s'", filename);

    // Encode according to file ending
    int len = strlen(filename);
    BOOL retval = FALSE;
    DEBUG("Comparing '%s' to known file endings", &filename[len-4]);
    if (0 == strncmp(&filename[len-4], ".png", 3))
    {
        DEBUG("Encoding image as PNG");
        retval = _encodePNG(&buffer, &buffersize, self);
    }
    else
    {
        DEBUG("No known file format, encoding image as PNG");
        retval = _encodePNG(&buffer, &buffersize, self);
    }

    if (!retval || NULL == buffer)
    {
        ERROR("Could not save image");
        return FALSE;
    }

    retval = _saveFile(filename, buffer, buffersize);

    return retval;
}
