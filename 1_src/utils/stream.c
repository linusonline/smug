#include <string.h>
#include <ctype.h>

#include <common.h>
#include <utils/general.h>
#include <utils/stream.h>

typedef struct _Stream {
	char* buffer;
	int position;
    BOOL copied;
} _String;

static BOOL isWsCharacter(char* c)
{
    return isspace(*c);
}

static BOOL _eof(char* c)
{
    return *c == NULL_CHAR;
}

static BOOL isWhiteSpaceOrEof(char* c)
{
    return _eof(c) || isWsCharacter(c);
}

Stream* Stream_new(char* string)
{
    Stream* newStream = allocate(Stream);
    newStream->buffer = string;
    newStream->position = 0;
    newStream->copied = FALSE;
    return newStream;
}

Stream* Stream_newCopied(char* string)
{
    Stream* newStream = Stream_new(copyString(string));
    newStream->copied = TRUE;
    return newStream;
}

void Stream_delete(Stream* self)
{
    if (self->copied)
    {
        free(self->buffer);
    }
    free(self);
}

BOOL Stream_eof(Stream* self)
{
    return _eof(self->buffer + self->position);
}

BOOL Stream_readToken(Stream* self, const char* token)
{
    Stream_eatWhiteSpace(self);
    int tokenLen = strlen(token);
    if (strncmp(self->buffer + self->position, token, tokenLen) == 0 && isWhiteSpaceOrEof(self->buffer + self->position + tokenLen))
    {
        self->position += tokenLen;
        Stream_eatWhiteSpace(self);
        return TRUE;
    }
    return FALSE;
}

void Stream_eatWhiteSpace(Stream* self)
{
    while (isWsCharacter(self->buffer + self->position) && !Stream_eof(self))
    {
        self->position++;
    }
}

char* Stream_readChars(Stream* self, int numChars)
{
    smug_assert(numChars > 0);
    smug_assert(numChars <= strlen(self->buffer + self->position));
    char* returnString = allocatev(char, numChars + 1);
    strncpy(returnString, self->buffer + self->position, numChars);
    returnString[numChars] = NULL_CHAR;
    self->position += numChars;
    return returnString;
}

char* Stream_readUntilWs(Stream* self)
{
    int numCharsRead = 0;
    while (!isWhiteSpaceOrEof(self->buffer + self->position + numCharsRead))
    {
        numCharsRead++;
    }
    char* chars = Stream_readChars(self, numCharsRead);
    Stream_eatWhiteSpace(self);
    return chars;
}

char* Stream_readString(Stream* self)
{
    Stream_eatWhiteSpace(self);
    if (self->buffer[self->position] == '\"')
    {
        int numCharsRead = 1;
        while (self->buffer[self->position + numCharsRead] != '\"')
        {
            if (_eof(self->buffer + self->position + numCharsRead))
            {
                ERROR("End of file while parsing string!");
                return NULL;
            }
            numCharsRead++;
        }
        numCharsRead++; // Read the final '"'
        char* chars = Stream_readChars(self, numCharsRead);
        Stream_eatWhiteSpace(self);
        return chars;
    }
    else
    {
        return Stream_readUntilWs(self);
    }
}
