#ifndef SMUG_UTILS_STREAM_H
#define SMUG_UTILS_STREAM_H

#include <common.h>

struct _Stream;
typedef struct _Stream Stream;

Stream* Stream_new(char* string);
Stream* Stream_newCopied(char* string);
void Stream_delete(Stream* self);

BOOL Stream_eof(Stream* self);
void Stream_eatWhiteSpace(Stream* self);

BOOL Stream_readToken(Stream* self, const char* token);
/**
 *  You may only use this if you are certain the stream has at least numChars
 *  characters remaining. numChars must be at least 1.
 */
char* Stream_readChars(Stream* self, int numChars);

char* Stream_readUntilWs(Stream* self);
char* Stream_readString(Stream* self);
int Stream_readInt(Stream* self);

#endif /* SMUG_UTILS_STREAM_H */
