#include <GL/glfw.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

#include <graphics/renderqueue.h>
#include <graphics/spritesheet.h>
#include <graphics/sprite.h>
#include <graphics/spriteanimation.h>
#include <graphics/camera.h>
#include <graphics/graphics.h>
#include <engine/gameobject.h>
#include <input/input.h>
#include <utils/log.h>
#include <utils/stdout_console.h>
#include <common.h>

static const int INITIAL_WINDOW_WIDTH = 640;
static const int INITIAL_WINDOW_HEIGHT = 480;
static int windowWidth = 640;
static int windowHeight = 480;

static SpriteSheet* landscapeSheet = NULL;
static SpriteSheet* buildingsSheet = NULL;
static SpriteSheet* cursorSheet = NULL;
static RenderQueue* renderQueue = NULL;

static GameObject** world;
static GameObject* cursor;

static Sprite** landscapeSprites;
static Sprite** buildingSprites;
static Sprite* cursorSprite;
static SpriteAnimation* animHouse;

#define BG_COUNT (15*20)
#define BUILDINGS_COUNT 13
static const int TOTAL_OBJECT_COUNT = BG_COUNT + BUILDINGS_COUNT + 1;
static const int SPRITE_COUNT = 23;

static Console* console = NULL;

static Controller* theController = NULL;

static Camera* camera;

#define BUTTON_UP 0
#define BUTTON_DOWN 1
#define BUTTON_LEFT 2
#define BUTTON_RIGHT 3
#define BUTTON_ENABLE_MOUSE 4
#define BUTTON_DISABLE_MOUSE 5
#define BUTTON_PAN_UP 6
#define BUTTON_PAN_DOWN 7
#define BUTTON_PAN_LEFT 8
#define BUTTON_PAN_RIGHT 9
#define MOUSE_POINTER 0

static int cursorPositionX;
static int cursorPositionY;
static const int U = 32; // Unit size
static const int WORLD_WIDTH = 640;     // This world happens to be just one screen big.
static const int WORLD_HEIGHT = 480;

static void drawStuff(RenderQueue* rq)
{
    for (int i = 0; i < TOTAL_OBJECT_COUNT; i++)
    {
        RenderQueue_addDrawable(rq, GameObject_getDrawable(world[i]), GameObject_getX(world[i]), GameObject_getY(world[i]));
    }
}

static void afterDrawing()
{
    RenderQueue_clear(renderQueue);
}

#define _newObjectFromSpriteAndDimensions(sprite, w, h, x, y) GameObject_newWithDrawable(x, y, Drawable_newFromSpriteAndSize(sprite, w, h), 0, 0)

static void createBackground()
{
    landscapeSheet = SpriteSheet_new("5_res/landscape.png", "5_res/landscape.txt");
    buildingsSheet = SpriteSheet_new("5_res/buildings.png", "5_res/buildings.txt");

    landscapeSprites = allocatev(Sprite*, SpriteSheet_getNrOfSprites(landscapeSheet));
    buildingSprites = allocatev(Sprite*, SpriteSheet_getNrOfSprites(buildingsSheet));

    for (int i = 0; i < SpriteSheet_getNrOfSprites(landscapeSheet); i++)
    {
        landscapeSprites[i] = SpriteSheet_getSprite(landscapeSheet, i);
    }
    for (int i = 0; i < SpriteSheet_getNrOfSprites(buildingsSheet); i++)
    {
        buildingSprites[i] = SpriteSheet_getSprite(buildingsSheet, i);
    }

    world = allocatev(GameObject*, TOTAL_OBJECT_COUNT);

    world[0] =  _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, 0, 0);
    world[1] =  _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U, 0);
    world[2] =  _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*2, 0);
    world[3] =  _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*3, 0);
    world[4] =  _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*4, 0);
    world[5] =  _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*5, 0);
    world[6] =  _newObjectFromSpriteAndDimensions(landscapeSprites[4], U, U, U*6, 0);
    world[7] =  _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*7, 0);
    world[8] =  _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*8, 0);
    world[9] =  _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*9, 0);
    world[10] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*10, 0);
    world[11] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*11, 0);
    world[12] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*12, 0);
    world[13] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*13, 0);
    world[14] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*14, 0);
    world[15] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*15, 0);
    world[16] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*16, 0);
    world[17] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*17, 0);
    world[18] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*18, 0);
    world[19] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*19, 0);

    world[20] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, 0, U);
    world[21] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U, U);
    world[22] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*2, U);
    world[23] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*3, U);
    world[24] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*4, U);
    world[25] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*5, U);
    world[26] = _newObjectFromSpriteAndDimensions(landscapeSprites[7], U, U, U*6, U);
    world[27] = _newObjectFromSpriteAndDimensions(landscapeSprites[6], U, U, U*7, U);
    world[28] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*8, U);
    world[29] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*9, U);
    world[30] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*10, U);
    world[31] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*11, U);
    world[32] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*12, U);
    world[33] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*13, U);
    world[34] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*14, U);
    world[35] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*15, U);
    world[36] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*16, U);
    world[37] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*17, U);
    world[38] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*18, U);
    world[39] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*19, U);

    world[40] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, 0, U*2);
    world[41] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U, U*2);
    world[42] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*2, U*2);
    world[43] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*3, U*2);
    world[44] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*4, U*2);
    world[45] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*5, U*2);
    world[46] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*6, U*2);
    world[47] = _newObjectFromSpriteAndDimensions(landscapeSprites[7], U, U, U*7, U*2);
    world[48] = _newObjectFromSpriteAndDimensions(landscapeSprites[3], U, U, U*8, U*2);
    world[49] = _newObjectFromSpriteAndDimensions(landscapeSprites[9], U, U, U*9, U*2);
    world[50] = _newObjectFromSpriteAndDimensions(landscapeSprites[3], U, U, U*10, U*2);
    world[51] = _newObjectFromSpriteAndDimensions(landscapeSprites[3], U, U, U*11, U*2);
    world[52] = _newObjectFromSpriteAndDimensions(landscapeSprites[3], U, U, U*12, U*2);
    world[53] = _newObjectFromSpriteAndDimensions(landscapeSprites[3], U, U, U*13, U*2);
    world[54] = _newObjectFromSpriteAndDimensions(landscapeSprites[3], U, U, U*14, U*2);
    world[55] = _newObjectFromSpriteAndDimensions(landscapeSprites[3], U, U, U*15, U*2);
    world[56] = _newObjectFromSpriteAndDimensions(landscapeSprites[3], U, U, U*16, U*2);
    world[57] = _newObjectFromSpriteAndDimensions(landscapeSprites[3], U, U, U*17, U*2);
    world[58] = _newObjectFromSpriteAndDimensions(landscapeSprites[3], U, U, U*18, U*2);
    world[59] = _newObjectFromSpriteAndDimensions(landscapeSprites[3], U, U, U*19, U*2);

    world[60] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, 0, U*3);
    world[61] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U, U*3);
    world[62] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*2, U*3);
    world[63] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*3, U*3);
    world[64] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*4, U*3);
    world[65] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*5, U*3);
    world[66] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*6, U*3);
    world[67] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*7, U*3);
    world[68] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*8, U*3);
    world[69] = _newObjectFromSpriteAndDimensions(landscapeSprites[21], U, U, U*9, U*3);
    world[70] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*10, U*3);
    world[71] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*11, U*3);
    world[72] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*12, U*3);
    world[73] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*13, U*3);
    world[74] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*14, U*3);
    world[75] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*15, U*3);
    world[76] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*16, U*3);
    world[77] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*17, U*3);
    world[78] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*18, U*3);
    world[79] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*19, U*3);

    world[80] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, 0, U*4);
    world[81] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U, U*4);
    world[82] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*2, U*4);
    world[83] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*3, U*4);
    world[84] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*4, U*4);
    world[85] = _newObjectFromSpriteAndDimensions(landscapeSprites[2], U, U, U*5, U*4);
    world[86] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*6, U*4);
    world[87] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*7, U*4);
    world[88] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*8, U*4);
    world[89] = _newObjectFromSpriteAndDimensions(landscapeSprites[4], U, U, U*9, U*4);
    world[90] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*10, U*4);
    world[91] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*11, U*4);
    world[92] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*12, U*4);
    world[93] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*13, U*4);
    world[94] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*14, U*4);
    world[95] = _newObjectFromSpriteAndDimensions(landscapeSprites[2], U, U, U*15, U*4);
    world[96] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*16, U*4);
    world[97] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*17, U*4);
    world[98] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*18, U*4);
    world[99] = _newObjectFromSpriteAndDimensions(landscapeSprites[2], U, U, U*19, U*4);

    world[100] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, 0, U*5);
    world[101] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U, U*5);
    world[102] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*2, U*5);
    world[103] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*3, U*5);
    world[104] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*4, U*5);
    world[105] = _newObjectFromSpriteAndDimensions(landscapeSprites[2], U, U, U*5, U*5);
    world[106] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*6, U*5);
    world[107] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*7, U*5);
    world[108] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*8, U*5);
    world[109] = _newObjectFromSpriteAndDimensions(landscapeSprites[7], U, U, U*9, U*5);
    world[110] = _newObjectFromSpriteAndDimensions(landscapeSprites[9], U, U, U*10, U*5);
    world[111] = _newObjectFromSpriteAndDimensions(landscapeSprites[3], U, U, U*11, U*5);
    world[112] = _newObjectFromSpriteAndDimensions(landscapeSprites[3], U, U, U*12, U*5);
    world[113] = _newObjectFromSpriteAndDimensions(landscapeSprites[3], U, U, U*13, U*5);
    world[114] = _newObjectFromSpriteAndDimensions(landscapeSprites[3], U, U, U*14, U*5);
    world[115] = _newObjectFromSpriteAndDimensions(landscapeSprites[3], U, U, U*15, U*5);
    world[116] = _newObjectFromSpriteAndDimensions(landscapeSprites[3], U, U, U*16, U*5);
    world[117] = _newObjectFromSpriteAndDimensions(landscapeSprites[3], U, U, U*17, U*5);
    world[118] = _newObjectFromSpriteAndDimensions(landscapeSprites[3], U, U, U*18, U*5);
    world[119] = _newObjectFromSpriteAndDimensions(landscapeSprites[3], U, U, U*19, U*5);

    world[120] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, 0, U*6);
    world[121] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U, U*6);
    world[122] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*2, U*6);
    world[123] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*3, U*6);
    world[124] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*4, U*6);
    world[125] = _newObjectFromSpriteAndDimensions(landscapeSprites[2], U, U, U*5, U*6);
    world[126] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*6, U*6);
    world[127] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*7, U*6);
    world[128] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*8, U*6);
    world[129] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*9, U*6);
    world[130] = _newObjectFromSpriteAndDimensions(landscapeSprites[4], U, U, U*10, U*6);
    world[131] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*11, U*6);
    world[132] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*12, U*6);
    world[133] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*13, U*6);
    world[134] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*14, U*6);
    world[135] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*15, U*6);
    world[136] = _newObjectFromSpriteAndDimensions(landscapeSprites[2], U, U, U*16, U*6);
    world[137] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*17, U*6);
    world[138] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*18, U*6);
    world[139] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*19, U*6);

    world[140] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, 0, U*7);
    world[141] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U, U*7);
    world[142] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*2, U*7);
    world[143] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*3, U*7);
    world[144] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*4, U*7);
    world[145] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*5, U*7);
    world[146] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*6, U*7);
    world[147] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*7, U*7);
    world[148] = _newObjectFromSpriteAndDimensions(landscapeSprites[5], U, U, U*8, U*7);
    world[149] = _newObjectFromSpriteAndDimensions(landscapeSprites[3], U, U, U*9, U*7);
    world[150] = _newObjectFromSpriteAndDimensions(landscapeSprites[8], U, U, U*10, U*7);
    world[151] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*11, U*7);
    world[152] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*12, U*7);
    world[153] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*13, U*7);
    world[154] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*14, U*7);
    world[155] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*15, U*7);
    world[156] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*16, U*7);
    world[157] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*17, U*7);
    world[158] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*18, U*7);
    world[159] = _newObjectFromSpriteAndDimensions(landscapeSprites[2], U, U, U*19, U*7);

    world[160] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, 0, U*8);
    world[161] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U, U*8);
    world[162] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*2, U*8);
    world[163] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*3, U*8);
    world[164] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*4, U*8);
    world[165] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*5, U*8);
    world[166] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*6, U*8);
    world[167] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*7, U*8);
    world[168] = _newObjectFromSpriteAndDimensions(landscapeSprites[4], U, U, U*8, U*8);
    world[169] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*9, U*8);
    world[170] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*10, U*8);
    world[171] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*11, U*8);
    world[172] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*12, U*8);
    world[173] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*13, U*8);
    world[174] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*14, U*8);
    world[175] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*15, U*8);
    world[176] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*16, U*8);
    world[177] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*17, U*8);
    world[178] = _newObjectFromSpriteAndDimensions(landscapeSprites[2], U, U, U*18, U*8);
    world[179] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*19, U*8);

    world[180] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, 0, U*9);
    world[181] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U, U*9);
    world[182] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*2, U*9);
    world[183] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*3, U*9);
    world[184] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*4, U*9);
    world[185] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*5, U*9);
    world[186] = _newObjectFromSpriteAndDimensions(landscapeSprites[5], U, U, U*6, U*9);
    world[187] = _newObjectFromSpriteAndDimensions(landscapeSprites[3], U, U, U*7, U*9);
    world[188] = _newObjectFromSpriteAndDimensions(landscapeSprites[8], U, U, U*8, U*9);
    world[189] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*9, U*9);
    world[190] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*10, U*9);
    world[191] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*11, U*9);
    world[192] = _newObjectFromSpriteAndDimensions(landscapeSprites[2], U, U, U*12, U*9);
    world[193] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*13, U*9);
    world[194] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*14, U*9);
    world[195] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*15, U*9);
    world[196] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*16, U*9);
    world[197] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*17, U*9);
    world[198] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*18, U*9);
    world[199] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*19, U*9);

    world[200] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, 0, U*10);
    world[201] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U, U*10);
    world[202] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*2, U*10);
    world[203] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*3, U*10);
    world[204] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*4, U*10);
    world[205] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*5, U*10);
    world[206] = _newObjectFromSpriteAndDimensions(landscapeSprites[4], U, U, U*6, U*10);
    world[207] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*7, U*10);
    world[208] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*8, U*10);
    world[209] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*9, U*10);
    world[210] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*10, U*10);
    world[211] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*11, U*10);
    world[212] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*12, U*10);
    world[213] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*13, U*10);
    world[214] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*14, U*10);
    world[215] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*15, U*10);
    world[216] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*16, U*10);
    world[217] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*17, U*10);
    world[218] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*18, U*10);
    world[219] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*19, U*10);

    world[220] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, 0, U*11);
    world[221] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U, U*11);
    world[222] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*2, U*11);
    world[223] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*3, U*11);
    world[224] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*4, U*11);
    world[225] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*5, U*11);
    world[226] = _newObjectFromSpriteAndDimensions(landscapeSprites[4], U, U, U*6, U*11);
    world[227] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*7, U*11);
    world[228] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*8, U*11);
    world[229] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*9, U*11);
    world[230] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*10, U*11);
    world[231] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*11, U*11);
    world[232] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*12, U*11);
    world[233] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*13, U*11);
    world[234] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*14, U*11);
    world[235] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*15, U*11);
    world[236] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*16, U*11);
    world[237] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*17, U*11);
    world[238] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*18, U*11);
    world[239] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*19, U*11);

    world[240] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, 0, U*12);
    world[241] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U, U*12);
    world[242] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*2, U*12);
    world[243] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*3, U*12);
    world[244] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*4, U*12);
    world[245] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*5, U*12);
    world[246] = _newObjectFromSpriteAndDimensions(landscapeSprites[4], U, U, U*6, U*12);
    world[247] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*7, U*12);
    world[248] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*8, U*12);
    world[249] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*9, U*12);
    world[250] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*10, U*12);
    world[251] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*11, U*12);
    world[252] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*12, U*12);
    world[253] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*13, U*12);
    world[254] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*14, U*12);
    world[255] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*15, U*12);
    world[256] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*16, U*12);
    world[257] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*17, U*12);
    world[258] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*18, U*12);
    world[259] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*19, U*12);

    world[260] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, 0, U*13);
    world[261] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U, U*13);
    world[262] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*2, U*13);
    world[263] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*3, U*13);
    world[264] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*4, U*13);
    world[265] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*5, U*13);
    world[266] = _newObjectFromSpriteAndDimensions(landscapeSprites[4], U, U, U*6, U*13);
    world[267] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*7, U*13);
    world[268] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*8, U*13);
    world[269] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*9, U*13);
    world[270] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*10, U*13);
    world[271] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*11, U*13);
    world[272] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*12, U*13);
    world[273] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*13, U*13);
    world[274] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*14, U*13);
    world[275] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*15, U*13);
    world[276] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*16, U*13);
    world[277] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*17, U*13);
    world[278] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*18, U*13);
    world[279] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*19, U*13);

    world[280] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, 0, U*14);
    world[281] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U, U*14);
    world[282] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*2, U*14);
    world[283] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*3, U*14);
    world[284] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*4, U*14);
    world[285] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*5, U*14);
    world[286] = _newObjectFromSpriteAndDimensions(landscapeSprites[4], U, U, U*6, U*14);
    world[287] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*7, U*14);
    world[288] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*8, U*14);
    world[289] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*9, U*14);
    world[290] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*10, U*14);
    world[291] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*11, U*14);
    world[292] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*12, U*14);
    world[293] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*13, U*14);
    world[294] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*14, U*14);
    world[295] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*15, U*14);
    world[296] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*16, U*14);
    world[297] = _newObjectFromSpriteAndDimensions(landscapeSprites[0], U, U, U*17, U*14);
    world[298] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*18, U*14);
    world[299] = _newObjectFromSpriteAndDimensions(landscapeSprites[1], U, U, U*19, U*14);

    world[300] =  _newObjectFromSpriteAndDimensions(buildingSprites[45], 40, 64, U*9, -U);
    world[301] = _newObjectFromSpriteAndDimensions(buildingSprites[45], 40, 64, U*10, -U);
    world[302] = _newObjectFromSpriteAndDimensions(buildingSprites[45], 40, 64, U*11, -U);
    world[303] = _newObjectFromSpriteAndDimensions(buildingSprites[45], 40, 64, U*12, -U);
    world[304] = _newObjectFromSpriteAndDimensions(buildingSprites[45], 40, 64, U*13, -U);
    world[305] = _newObjectFromSpriteAndDimensions(buildingSprites[45], 40, 64, U*2, U);
    world[306] = _newObjectFromSpriteAndDimensions(buildingSprites[3], 40, 64, U*2, U*5);
    world[307] = _newObjectFromSpriteAndDimensions(buildingSprites[5], 40, 64, U*4, U*5);
    world[308] = _newObjectFromSpriteAndDimensions(buildingSprites[3], 40, 64, U, U*6);
    world[309] = _newObjectFromSpriteAndDimensions(buildingSprites[0], 40, 64, U*2, U*6);
    world[310] = _newObjectFromSpriteAndDimensions(buildingSprites[3], 40, 64, U*3, U*6);
    world[311] = _newObjectFromSpriteAndDimensions(buildingSprites[3], 40, 64, U*2, U*7);

    animHouse = SpriteAnimation_newEmpty();
    SpriteAnimation_addFrame(animHouse, buildingSprites[10], 0.5);
    SpriteAnimation_addFrame(animHouse, buildingSprites[11], 0.5);
    world[312] = GameObject_new(U*4, U*6);
    GameObject_addDrawableAt(world[312], Drawable_newFromSpriteAnimationAndSize(animHouse, 40, 64), 0.0, 0.0);
    SpriteAnimation_start(animHouse);
}

static void createCursor(int posX, int posY)
{
    cursorSheet = SpriteSheet_new("5_res/cursor.png", NULL);
    cursorSprite = SpriteSheet_getSprite(cursorSheet, 0);
    cursor = GameObject_new(posX*U, posY*U);
    GameObject_addDrawableAt(cursor, Drawable_newFromSpriteAndSize(cursorSprite, U, U), 0.0, 0.0);
    world[313] = cursor;
}

static void moveCursor(int keyid)
{
    switch (keyid)
    {
        case BUTTON_UP:
            if (GameObject_getY(cursor) >= U)
            {
                GameObject_setPos(cursor, GameObject_getX(cursor), GameObject_getY(cursor) - U);
                if (abs(GameObject_getY(cursor) - camera->posY) > windowHeight/4)
                {
                    camera->posY -= U;
                }
            }
            break;
        case BUTTON_DOWN:
            if (GameObject_getY(cursor) <= (WORLD_HEIGHT - U*2))
            {
                GameObject_setPos(cursor, GameObject_getX(cursor), GameObject_getY(cursor) + U);
                if (abs(GameObject_getY(cursor) - camera->posY) > windowHeight/4)
                {
                    camera->posY += U;
                }
            }
            break;
        case BUTTON_LEFT:
            if (GameObject_getX(cursor) >= U)
            {
                GameObject_setPos(cursor, GameObject_getX(cursor) - U, GameObject_getY(cursor));
                if (abs(GameObject_getX(cursor) - camera->posX) > windowWidth/4)
                {
                    camera->posX -= U;
                }
            }
            break;
        case BUTTON_RIGHT:
            if (GameObject_getX(cursor) <= (WORLD_WIDTH - U*2))
            {
                GameObject_setPos(cursor, GameObject_getX(cursor) + U, GameObject_getY(cursor));
                if (abs(GameObject_getX(cursor) - camera->posX) > windowWidth/4)
                {
                    camera->posX += U;
                }
            }
            break;
    }
}

static void useMouse(BOOL use)
{
    if (use)
    {
        Input_linkControllerToMousePosition(theController, MOUSE_POINTER);
    }
    else
    {
        Input_unlinkControllersFromMousePosition();
    }
}

static BOOL _pointerCallback(Controller* controller, int pointerIndex, int xDelta, int yDelta)
{
    DEBUG("Pointer moved to %i, %i", xDelta, yDelta);
    return TRUE;
}

static BOOL _buttonCallback(Controller* controller, int buttonIndex, int state)
{
    smug_assert(controller == theController);
    if (state == SMUG_KEY_RELEASE)
    {
        return TRUE;
    }
    switch (buttonIndex)
    {
        case BUTTON_ENABLE_MOUSE:
            useMouse(TRUE); break;
        case BUTTON_DISABLE_MOUSE:
            useMouse(FALSE); break;
        case BUTTON_UP:
        case BUTTON_DOWN:
        case BUTTON_LEFT:
        case BUTTON_RIGHT:
            moveCursor(buttonIndex); break;
        case BUTTON_PAN_UP:
        case BUTTON_PAN_DOWN:
        case BUTTON_PAN_LEFT:
        case BUTTON_PAN_RIGHT:
            SMUG_NOOP(); break;
        default:
            smug_assert(FALSE);
    }
    return TRUE;
}

static void _resizeCallback(int width, int height)
{
    windowWidth = width;
    windowHeight = height;
}

static void init()
{
    console = StdoutConsole_new();
    smug_assert(console != NULL);
    Log_init(console);
    DEBUG("==============================");
    DEBUG("");
    DEBUG("Input demo");
    DEBUG("");
    DEBUG("Press arrow keys to move cursor.");
    DEBUG("Press home/end to enable/disable mouse.");
    DEBUG("");
    DEBUG("==============================");

    camera = Camera_new();
    camera->posX = INITIAL_WINDOW_WIDTH / 2;
    camera->posY = INITIAL_WINDOW_HEIGHT / 2;

    Graphics_initialize(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, FALSE);
    Graphics_setCoordinateSystemForWindow(-WORLD_WIDTH/2, -WORLD_HEIGHT/2, WORLD_WIDTH, WORLD_HEIGHT);
    Graphics_setBackgroundColor(0.5, 0.0, 0.5);
    Graphics_useCamera(camera);
    Graphics_setWindowResizeCallback(_resizeCallback);
    // setCoordinateSystemInPixelsPerUnit(1.3, 1.3, 20.0, 20.0);

    Input_initialize();
    theController = Controller_new(0, 10, 1);
    Input_setButtonCallbackForController(theController, _buttonCallback);
    Input_setPointerCallbackForController(theController, _pointerCallback);
    Input_linkControllerToKeyboardKey(theController, BUTTON_UP, GLFW_KEY_UP);
    Input_linkControllerToKeyboardKey(theController, BUTTON_DOWN, GLFW_KEY_DOWN);
    Input_linkControllerToKeyboardKey(theController, BUTTON_LEFT, GLFW_KEY_LEFT);
    Input_linkControllerToKeyboardKey(theController, BUTTON_RIGHT, GLFW_KEY_RIGHT);
    Input_linkControllerToKeyboardKey(theController, BUTTON_ENABLE_MOUSE, GLFW_KEY_HOME);
    Input_linkControllerToKeyboardKey(theController, BUTTON_DISABLE_MOUSE, GLFW_KEY_END);
    Input_linkControllerToKeyboardKey(theController, BUTTON_PAN_UP, 'W');
    Input_linkControllerToKeyboardKey(theController, BUTTON_PAN_DOWN, 'S');
    Input_linkControllerToKeyboardKey(theController, BUTTON_PAN_LEFT, 'A');
    Input_linkControllerToKeyboardKey(theController, BUTTON_PAN_RIGHT, 'D');

    renderQueue = RenderQueue_new();

    createBackground();
    cursorPositionX = 10;
    cursorPositionY = 8;
    createCursor(cursorPositionX, cursorPositionY);
}

static void runMainLoop()
{
    BOOL running = TRUE;
    while (running)
    {
        drawStuff(renderQueue);
        Graphics_render(renderQueue);

        // Swap front and back rendering buffers
        Graphics_refreshWindow();

        afterDrawing();

        // Check if ESC key was pressed or window was closed
        running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
    }
}

static void deinit()
{
    RenderQueue_delete(renderQueue);

    for (int i = 0; i < TOTAL_OBJECT_COUNT; i++)
    {
        GameObject_delete(world[i]);
    }

    SpriteAnimation_delete(animHouse);
    SpriteSheet_delete(landscapeSheet);
    SpriteSheet_delete(buildingsSheet);

    Input_unlinkControllersFromKeyboardKey(GLFW_KEY_UP);
    Input_unlinkControllersFromKeyboardKey(GLFW_KEY_DOWN);
    Input_unlinkControllersFromKeyboardKey(GLFW_KEY_LEFT);
    Input_unlinkControllersFromKeyboardKey(GLFW_KEY_RIGHT);
    Input_unlinkControllersFromKeyboardKey(GLFW_KEY_HOME);
    Input_unlinkControllersFromKeyboardKey(GLFW_KEY_END);
    useMouse(FALSE);
    Controller_delete(theController);

    Graphics_terminate();
    Input_terminate();
    Log_terminate();
    StdoutConsole_delete(console);
}

int main()
{
    init();
    runMainLoop();
    deinit();
    return 0;
}
