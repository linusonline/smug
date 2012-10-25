#include <actiongauge.h>

#include <engine/gameobject.h>
#include <engine/engine.h>
#include <graphics/drawable.h>
#include <graphics/spritesheet.h>
#include <graphics/sprite.h>

static SpriteSheet* guiSheet = NULL;
static GameObject* actionGauge = NULL;
static GameObject* actionGaugeFrame = NULL;
static Drawable* gaugeDrawable = NULL;
static float MAX_WIDTH = 24;
static float SPACE_UNDER_AVATAR = 8;

void setActionGaugeValue(float value)
{
    Drawable_setSize(gaugeDrawable, MAX_WIDTH * value / 100.0f, 4);
}

void createActionGauge(float posX, float posY)
{
    guiSheet = SpriteSheet_new("res/gui/gauge.png", "res/gui/gauge.txt");
    Drawable* d = Drawable_newFromSpriteAndSize(SpriteSheet_getSprite(guiSheet, 0), 32, 8);
    actionGaugeFrame = GameObject_new(posX, posY);
    Drawable_setZ(d, 2000);
    GameObject_addDrawableAt(actionGaugeFrame, d, -16, SPACE_UNDER_AVATAR);

    actionGauge = GameObject_new(0, 0);
    gaugeDrawable = Drawable_newFromColorAndSize(0.5, 0.2, 0.0, 1.0, 24, 4);
    Drawable_setZ(gaugeDrawable, 2001);
    GameObject_addDrawableAt(actionGauge, gaugeDrawable, -12, SPACE_UNDER_AVATAR + 2);
    GameObject_addObject(actionGaugeFrame, actionGauge);

    Engine_addObject(actionGauge);
    Engine_addObject(actionGaugeFrame);
}

void setActionGaugePosition(float x, float y)
{
    GameObject_setPos(actionGaugeFrame, x, y);
}

void deleteActionGauge()
{
    Engine_removeObject(actionGauge);
    Engine_removeObject(actionGaugeFrame);
    GameObject_delete(actionGauge);
    GameObject_delete(actionGaugeFrame);
    SpriteSheet_delete(guiSheet);
}
