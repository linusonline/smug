#ifndef ELEMENTS_AVATAR_H
#define ELEMENTS_AVATAR_H

#include <common.h>

Drawable* getAvatar(float width, float height, float posX, float posY);
void deleteAvatar();

void avatarWalk(BOOL walk);
void setAvatarLeft();
void setAvatarRight();
void setAvatarUp();
void setAvatarDown();

#endif /* ELEMENTS_AVATAR_H */
