#include <GL/glfw.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <graphics/renderqueue.h>
#include <graphics/spritesheet.h>
#include <graphics/sprite.h>
#include <graphics/spriteanimation.h>
#include <input/input.h>
#include <utils/log.h>
#include <utils/stdout_console.h>
#include <common.h>

static SpriteSheet* landscapeSheet = NULL;
static SpriteSheet* buildingsSheet = NULL;
static SpriteSheet* cursorSheet = NULL;
static RenderQueue* renderQueue = NULL;

static Drawable** backgroundGraphics;
static Drawable** buildings;
static Drawable* cursor;
static Sprite** landscapeSprites;
static Sprite** buildingSprites;
static Sprite* cursorSprite;
static SpriteAnimation* animHouse;

static const int BG_COUNT = 15*20;
static const int BUILDINGS_COUNT = 13;
static const int SPRITE_COUNT = 23;

static Console* console = NULL;

static Controller* theController = NULL;

#define BUTTON_UP 0
#define BUTTON_DOWN 1
#define BUTTON_LEFT 2
#define BUTTON_RIGHT 3
#define BUTTON_ENABLE_MOUSE 4
#define BUTTON_DISABLE_MOUSE 5
#define MOUSE_POINTER 0

static void GLFWCALL windowResize(int width, int height)
{
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble)width, (GLdouble)height, 0.0);
}

static void drawStuff(RenderQueue* rq)
{
    for (int i = 0; i < BG_COUNT; i++)
    {
        RenderQueue_addDrawable(renderQueue, backgroundGraphics[i]);
    }
    for (int i = 0; i < BUILDINGS_COUNT; i++)
    {
        RenderQueue_addDrawable(renderQueue, buildings[i]);
    }
    RenderQueue_addDrawable(renderQueue, cursor);
}

static void afterDrawing()
{
    RenderQueue_clear(renderQueue);
}

static void glInit()
{
    // int glfwInit( void )
    if (glfwInit() != GL_TRUE)
    {
        exit(EXIT_FAILURE);
    }

    // int glfwOpenWindow( int width, int height,
    //      int redbits, int greenbits, int bluebits,
    //      int alphabits, int depthbits, int stencilbits,
    //      int mode )
    if (glfwOpenWindow(0, 0, 0, 0, 0, 0, 0, 0, GLFW_WINDOW) != GL_TRUE)
    {
        // void glfwTerminate( void )
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwSetWindowSizeCallback(windowResize);
    glClearColor(0.5, 0.0, 0.5, 0.0);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

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

    buildings = allocatev(Drawable*, BUILDINGS_COUNT);

    buildings[0] =  Drawable_newFromSpriteAndSize(buildingSprites[45], 40, 64, 288, -32);
    buildings[1] = Drawable_newFromSpriteAndSize(buildingSprites[45], 40, 64, 320, -32);
    buildings[2] = Drawable_newFromSpriteAndSize(buildingSprites[45], 40, 64, 352, -32);
    buildings[3] = Drawable_newFromSpriteAndSize(buildingSprites[45], 40, 64, 384, -32);
    buildings[4] = Drawable_newFromSpriteAndSize(buildingSprites[45], 40, 64, 416, -32);
    buildings[5] = Drawable_newFromSpriteAndSize(buildingSprites[45], 40, 64, 64, 32);
    buildings[6] = Drawable_newFromSpriteAndSize(buildingSprites[3], 40, 64, 64, 160);
    buildings[7] = Drawable_newFromSpriteAndSize(buildingSprites[5], 40, 64, 128, 160);
    buildings[8] = Drawable_newFromSpriteAndSize(buildingSprites[3], 40, 64, 32, 192);
    buildings[9] = Drawable_newFromSpriteAndSize(buildingSprites[0], 40, 64, 64, 192);
    buildings[10] = Drawable_newFromSpriteAndSize(buildingSprites[3], 40, 64, 96, 192);
    buildings[11] = Drawable_newFromSpriteAndSize(buildingSprites[3], 40, 64, 64, 224);

    animHouse = SpriteAnimation_newEmpty();
    SpriteAnimation_addFrame(animHouse, buildingSprites[10], 0.5);
    SpriteAnimation_addFrame(animHouse, buildingSprites[11], 0.5);
    buildings[12] = Drawable_newFromSpriteAnimationAndSize(animHouse, 40, 64, 128, 192);
    SpriteAnimation_start(animHouse);

    backgroundGraphics = allocatev(Drawable*, 20 * 15);

    backgroundGraphics[0] =  Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 0, 0);
    backgroundGraphics[1] =  Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 32, 0);
    backgroundGraphics[2] =  Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 64, 0);
    backgroundGraphics[3] =  Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 96, 0);
    backgroundGraphics[4] =  Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 128, 0);
    backgroundGraphics[5] =  Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 160, 0);
    backgroundGraphics[6] =  Drawable_newFromSpriteAndSize(landscapeSprites[4], 32, 32, 192, 0);
    backgroundGraphics[7] =  Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 224, 0);
    backgroundGraphics[8] =  Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 256, 0);
    backgroundGraphics[9] =  Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 288, 0);
    backgroundGraphics[10] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 320, 0);
    backgroundGraphics[11] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 352, 0);
    backgroundGraphics[12] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 384, 0);
    backgroundGraphics[13] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 416, 0);
    backgroundGraphics[14] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 448, 0);
    backgroundGraphics[15] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 480, 0);
    backgroundGraphics[16] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 512, 0);
    backgroundGraphics[17] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 544, 0);
    backgroundGraphics[18] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 576, 0);
    backgroundGraphics[19] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 608, 0);

    backgroundGraphics[20] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 0, 32);
    backgroundGraphics[21] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 32, 32);
    backgroundGraphics[22] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 64, 32);
    backgroundGraphics[23] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 96, 32);
    backgroundGraphics[24] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 128, 32);
    backgroundGraphics[25] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 160, 32);
    backgroundGraphics[26] = Drawable_newFromSpriteAndSize(landscapeSprites[7], 32, 32, 192, 32);
    backgroundGraphics[27] = Drawable_newFromSpriteAndSize(landscapeSprites[6], 32, 32, 224, 32);
    backgroundGraphics[28] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 256, 32);
    backgroundGraphics[29] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 288, 32);
    backgroundGraphics[30] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 320, 32);
    backgroundGraphics[31] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 352, 32);
    backgroundGraphics[32] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 384, 32);
    backgroundGraphics[33] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 416, 32);
    backgroundGraphics[34] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 448, 32);
    backgroundGraphics[35] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 480, 32);
    backgroundGraphics[36] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 512, 32);
    backgroundGraphics[37] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 544, 32);
    backgroundGraphics[38] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 576, 32);
    backgroundGraphics[39] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 608, 32);

    backgroundGraphics[40] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 0, 64);
    backgroundGraphics[41] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 32, 64);
    backgroundGraphics[42] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 64, 64);
    backgroundGraphics[43] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 96, 64);
    backgroundGraphics[44] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 128, 64);
    backgroundGraphics[45] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 160, 64);
    backgroundGraphics[46] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 192, 64);
    backgroundGraphics[47] = Drawable_newFromSpriteAndSize(landscapeSprites[7], 32, 32, 224, 64);
    backgroundGraphics[48] = Drawable_newFromSpriteAndSize(landscapeSprites[3], 32, 32, 256, 64);
    backgroundGraphics[49] = Drawable_newFromSpriteAndSize(landscapeSprites[9], 32, 32, 288, 64);
    backgroundGraphics[50] = Drawable_newFromSpriteAndSize(landscapeSprites[3], 32, 32, 320, 64);
    backgroundGraphics[51] = Drawable_newFromSpriteAndSize(landscapeSprites[3], 32, 32, 352, 64);
    backgroundGraphics[52] = Drawable_newFromSpriteAndSize(landscapeSprites[3], 32, 32, 384, 64);
    backgroundGraphics[53] = Drawable_newFromSpriteAndSize(landscapeSprites[3], 32, 32, 416, 64);
    backgroundGraphics[54] = Drawable_newFromSpriteAndSize(landscapeSprites[3], 32, 32, 448, 64);
    backgroundGraphics[55] = Drawable_newFromSpriteAndSize(landscapeSprites[3], 32, 32, 480, 64);
    backgroundGraphics[56] = Drawable_newFromSpriteAndSize(landscapeSprites[3], 32, 32, 512, 64);
    backgroundGraphics[57] = Drawable_newFromSpriteAndSize(landscapeSprites[3], 32, 32, 544, 64);
    backgroundGraphics[58] = Drawable_newFromSpriteAndSize(landscapeSprites[3], 32, 32, 576, 64);
    backgroundGraphics[59] = Drawable_newFromSpriteAndSize(landscapeSprites[3], 32, 32, 608, 64);

    backgroundGraphics[60] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 0, 96);
    backgroundGraphics[61] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 32, 96);
    backgroundGraphics[62] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 64, 96);
    backgroundGraphics[63] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 96, 96);
    backgroundGraphics[64] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 128, 96);
    backgroundGraphics[65] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 160, 96);
    backgroundGraphics[66] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 192, 96);
    backgroundGraphics[67] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 224, 96);
    backgroundGraphics[68] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 256, 96);
    backgroundGraphics[69] = Drawable_newFromSpriteAndSize(landscapeSprites[21], 32, 32, 288, 96);
    backgroundGraphics[70] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 320, 96);
    backgroundGraphics[71] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 352, 96);
    backgroundGraphics[72] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 384, 96);
    backgroundGraphics[73] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 416, 96);
    backgroundGraphics[74] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 448, 96);
    backgroundGraphics[75] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 480, 96);
    backgroundGraphics[76] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 512, 96);
    backgroundGraphics[77] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 544, 96);
    backgroundGraphics[78] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 576, 96);
    backgroundGraphics[79] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 608, 96);

    backgroundGraphics[80] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 0, 128);
    backgroundGraphics[81] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 32, 128);
    backgroundGraphics[82] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 64, 128);
    backgroundGraphics[83] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 96, 128);
    backgroundGraphics[84] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 128, 128);
    backgroundGraphics[85] = Drawable_newFromSpriteAndSize(landscapeSprites[2], 32, 32, 160, 128);
    backgroundGraphics[86] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 192, 128);
    backgroundGraphics[87] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 224, 128);
    backgroundGraphics[88] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 256, 128);
    backgroundGraphics[89] = Drawable_newFromSpriteAndSize(landscapeSprites[4], 32, 32, 288, 128);
    backgroundGraphics[90] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 320, 128);
    backgroundGraphics[91] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 352, 128);
    backgroundGraphics[92] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 384, 128);
    backgroundGraphics[93] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 416, 128);
    backgroundGraphics[94] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 448, 128);
    backgroundGraphics[95] = Drawable_newFromSpriteAndSize(landscapeSprites[2], 32, 32, 480, 128);
    backgroundGraphics[96] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 512, 128);
    backgroundGraphics[97] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 544, 128);
    backgroundGraphics[98] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 576, 128);
    backgroundGraphics[99] = Drawable_newFromSpriteAndSize(landscapeSprites[2], 32, 32, 608, 128);

    backgroundGraphics[100] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 0, 160);
    backgroundGraphics[101] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 32, 160);
    backgroundGraphics[102] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 64, 160);
    backgroundGraphics[103] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 96, 160);
    backgroundGraphics[104] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 128, 160);
    backgroundGraphics[105] = Drawable_newFromSpriteAndSize(landscapeSprites[2], 32, 32, 160, 160);
    backgroundGraphics[106] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 192, 160);
    backgroundGraphics[107] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 224, 160);
    backgroundGraphics[108] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 256, 160);
    backgroundGraphics[109] = Drawable_newFromSpriteAndSize(landscapeSprites[7], 32, 32, 288, 160);
    backgroundGraphics[110] = Drawable_newFromSpriteAndSize(landscapeSprites[9], 32, 32, 320, 160);
    backgroundGraphics[111] = Drawable_newFromSpriteAndSize(landscapeSprites[3], 32, 32, 352, 160);
    backgroundGraphics[112] = Drawable_newFromSpriteAndSize(landscapeSprites[3], 32, 32, 384, 160);
    backgroundGraphics[113] = Drawable_newFromSpriteAndSize(landscapeSprites[3], 32, 32, 416, 160);
    backgroundGraphics[114] = Drawable_newFromSpriteAndSize(landscapeSprites[3], 32, 32, 448, 160);
    backgroundGraphics[115] = Drawable_newFromSpriteAndSize(landscapeSprites[3], 32, 32, 480, 160);
    backgroundGraphics[116] = Drawable_newFromSpriteAndSize(landscapeSprites[3], 32, 32, 512, 160);
    backgroundGraphics[117] = Drawable_newFromSpriteAndSize(landscapeSprites[3], 32, 32, 544, 160);
    backgroundGraphics[118] = Drawable_newFromSpriteAndSize(landscapeSprites[3], 32, 32, 576, 160);
    backgroundGraphics[119] = Drawable_newFromSpriteAndSize(landscapeSprites[3], 32, 32, 608, 160);

    backgroundGraphics[120] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 0, 192);
    backgroundGraphics[121] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 32, 192);
    backgroundGraphics[122] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 64, 192);
    backgroundGraphics[123] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 96, 192);
    backgroundGraphics[124] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 128, 192);
    backgroundGraphics[125] = Drawable_newFromSpriteAndSize(landscapeSprites[2], 32, 32, 160, 192);
    backgroundGraphics[126] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 192, 192);
    backgroundGraphics[127] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 224, 192);
    backgroundGraphics[128] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 256, 192);
    backgroundGraphics[129] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 288, 192);
    backgroundGraphics[130] = Drawable_newFromSpriteAndSize(landscapeSprites[4], 32, 32, 320, 192);
    backgroundGraphics[131] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 352, 192);
    backgroundGraphics[132] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 384, 192);
    backgroundGraphics[133] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 416, 192);
    backgroundGraphics[134] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 448, 192);
    backgroundGraphics[135] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 480, 192);
    backgroundGraphics[136] = Drawable_newFromSpriteAndSize(landscapeSprites[2], 32, 32, 512, 192);
    backgroundGraphics[137] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 544, 192);
    backgroundGraphics[138] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 576, 192);
    backgroundGraphics[139] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 608, 192);

    backgroundGraphics[140] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 0, 224);
    backgroundGraphics[141] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 32, 224);
    backgroundGraphics[142] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 64, 224);
    backgroundGraphics[143] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 96, 224);
    backgroundGraphics[144] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 128, 224);
    backgroundGraphics[145] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 160, 224);
    backgroundGraphics[146] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 192, 224);
    backgroundGraphics[147] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 224, 224);
    backgroundGraphics[148] = Drawable_newFromSpriteAndSize(landscapeSprites[5], 32, 32, 256, 224);
    backgroundGraphics[149] = Drawable_newFromSpriteAndSize(landscapeSprites[3], 32, 32, 288, 224);
    backgroundGraphics[150] = Drawable_newFromSpriteAndSize(landscapeSprites[8], 32, 32, 320, 224);
    backgroundGraphics[151] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 352, 224);
    backgroundGraphics[152] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 384, 224);
    backgroundGraphics[153] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 416, 224);
    backgroundGraphics[154] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 448, 224);
    backgroundGraphics[155] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 480, 224);
    backgroundGraphics[156] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 512, 224);
    backgroundGraphics[157] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 544, 224);
    backgroundGraphics[158] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 576, 224);
    backgroundGraphics[159] = Drawable_newFromSpriteAndSize(landscapeSprites[2], 32, 32, 608, 224);

    backgroundGraphics[160] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 0, 256);
    backgroundGraphics[161] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 32, 256);
    backgroundGraphics[162] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 64, 256);
    backgroundGraphics[163] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 96, 256);
    backgroundGraphics[164] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 128, 256);
    backgroundGraphics[165] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 160, 256);
    backgroundGraphics[166] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 192, 256);
    backgroundGraphics[167] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 224, 256);
    backgroundGraphics[168] = Drawable_newFromSpriteAndSize(landscapeSprites[4], 32, 32, 256, 256);
    backgroundGraphics[169] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 288, 256);
    backgroundGraphics[170] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 320, 256);
    backgroundGraphics[171] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 352, 256);
    backgroundGraphics[172] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 384, 256);
    backgroundGraphics[173] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 416, 256);
    backgroundGraphics[174] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 448, 256);
    backgroundGraphics[175] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 480, 256);
    backgroundGraphics[176] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 512, 256);
    backgroundGraphics[177] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 544, 256);
    backgroundGraphics[178] = Drawable_newFromSpriteAndSize(landscapeSprites[2], 32, 32, 576, 256);
    backgroundGraphics[179] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 608, 256);

    backgroundGraphics[180] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 0, 288);
    backgroundGraphics[181] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 32, 288);
    backgroundGraphics[182] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 64, 288);
    backgroundGraphics[183] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 96, 288);
    backgroundGraphics[184] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 128, 288);
    backgroundGraphics[185] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 160, 288);
    backgroundGraphics[186] = Drawable_newFromSpriteAndSize(landscapeSprites[5], 32, 32, 192, 288);
    backgroundGraphics[187] = Drawable_newFromSpriteAndSize(landscapeSprites[3], 32, 32, 224, 288);
    backgroundGraphics[188] = Drawable_newFromSpriteAndSize(landscapeSprites[8], 32, 32, 256, 288);
    backgroundGraphics[189] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 288, 288);
    backgroundGraphics[190] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 320, 288);
    backgroundGraphics[191] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 352, 288);
    backgroundGraphics[192] = Drawable_newFromSpriteAndSize(landscapeSprites[2], 32, 32, 384, 288);
    backgroundGraphics[193] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 416, 288);
    backgroundGraphics[194] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 448, 288);
    backgroundGraphics[195] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 480, 288);
    backgroundGraphics[196] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 512, 288);
    backgroundGraphics[197] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 544, 288);
    backgroundGraphics[198] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 576, 288);
    backgroundGraphics[199] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 608, 288);

    backgroundGraphics[200] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 0, 320);
    backgroundGraphics[201] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 32, 320);
    backgroundGraphics[202] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 64, 320);
    backgroundGraphics[203] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 96, 320);
    backgroundGraphics[204] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 128, 320);
    backgroundGraphics[205] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 160, 320);
    backgroundGraphics[206] = Drawable_newFromSpriteAndSize(landscapeSprites[4], 32, 32, 192, 320);
    backgroundGraphics[207] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 224, 320);
    backgroundGraphics[208] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 256, 320);
    backgroundGraphics[209] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 288, 320);
    backgroundGraphics[210] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 320, 320);
    backgroundGraphics[211] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 352, 320);
    backgroundGraphics[212] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 384, 320);
    backgroundGraphics[213] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 416, 320);
    backgroundGraphics[214] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 448, 320);
    backgroundGraphics[215] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 480, 320);
    backgroundGraphics[216] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 512, 320);
    backgroundGraphics[217] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 544, 320);
    backgroundGraphics[218] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 576, 320);
    backgroundGraphics[219] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 608, 320);

    backgroundGraphics[220] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 0, 352);
    backgroundGraphics[221] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 32, 352);
    backgroundGraphics[222] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 64, 352);
    backgroundGraphics[223] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 96, 352);
    backgroundGraphics[224] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 128, 352);
    backgroundGraphics[225] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 160, 352);
    backgroundGraphics[226] = Drawable_newFromSpriteAndSize(landscapeSprites[4], 32, 32, 192, 352);
    backgroundGraphics[227] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 224, 352);
    backgroundGraphics[228] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 256, 352);
    backgroundGraphics[229] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 288, 352);
    backgroundGraphics[230] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 320, 352);
    backgroundGraphics[231] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 352, 352);
    backgroundGraphics[232] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 384, 352);
    backgroundGraphics[233] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 416, 352);
    backgroundGraphics[234] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 448, 352);
    backgroundGraphics[235] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 480, 352);
    backgroundGraphics[236] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 512, 352);
    backgroundGraphics[237] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 544, 352);
    backgroundGraphics[238] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 576, 352);
    backgroundGraphics[239] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 608, 352);

    backgroundGraphics[240] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 0, 384);
    backgroundGraphics[241] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 32, 384);
    backgroundGraphics[242] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 64, 384);
    backgroundGraphics[243] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 96, 384);
    backgroundGraphics[244] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 128, 384);
    backgroundGraphics[245] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 160, 384);
    backgroundGraphics[246] = Drawable_newFromSpriteAndSize(landscapeSprites[4], 32, 32, 192, 384);
    backgroundGraphics[247] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 224, 384);
    backgroundGraphics[248] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 256, 384);
    backgroundGraphics[249] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 288, 384);
    backgroundGraphics[250] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 320, 384);
    backgroundGraphics[251] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 352, 384);
    backgroundGraphics[252] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 384, 384);
    backgroundGraphics[253] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 416, 384);
    backgroundGraphics[254] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 448, 384);
    backgroundGraphics[255] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 480, 384);
    backgroundGraphics[256] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 512, 384);
    backgroundGraphics[257] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 544, 384);
    backgroundGraphics[258] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 576, 384);
    backgroundGraphics[259] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 608, 384);

    backgroundGraphics[260] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 0, 416);
    backgroundGraphics[261] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 32, 416);
    backgroundGraphics[262] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 64, 416);
    backgroundGraphics[263] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 96, 416);
    backgroundGraphics[264] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 128, 416);
    backgroundGraphics[265] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 160, 416);
    backgroundGraphics[266] = Drawable_newFromSpriteAndSize(landscapeSprites[4], 32, 32, 192, 416);
    backgroundGraphics[267] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 224, 416);
    backgroundGraphics[268] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 256, 416);
    backgroundGraphics[269] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 288, 416);
    backgroundGraphics[270] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 320, 416);
    backgroundGraphics[271] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 352, 416);
    backgroundGraphics[272] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 384, 416);
    backgroundGraphics[273] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 416, 416);
    backgroundGraphics[274] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 448, 416);
    backgroundGraphics[275] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 480, 416);
    backgroundGraphics[276] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 512, 416);
    backgroundGraphics[277] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 544, 416);
    backgroundGraphics[278] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 576, 416);
    backgroundGraphics[279] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 608, 416);

    backgroundGraphics[280] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 0, 448);
    backgroundGraphics[281] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 32, 448);
    backgroundGraphics[282] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 64, 448);
    backgroundGraphics[283] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 96, 448);
    backgroundGraphics[284] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 128, 448);
    backgroundGraphics[285] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 160, 448);
    backgroundGraphics[286] = Drawable_newFromSpriteAndSize(landscapeSprites[4], 32, 32, 192, 448);
    backgroundGraphics[287] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 224, 448);
    backgroundGraphics[288] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 256, 448);
    backgroundGraphics[289] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 288, 448);
    backgroundGraphics[290] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 320, 448);
    backgroundGraphics[291] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 352, 448);
    backgroundGraphics[292] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 384, 448);
    backgroundGraphics[293] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 416, 448);
    backgroundGraphics[294] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 448, 448);
    backgroundGraphics[295] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 480, 448);
    backgroundGraphics[296] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 512, 448);
    backgroundGraphics[297] = Drawable_newFromSpriteAndSize(landscapeSprites[0], 32, 32, 544, 448);
    backgroundGraphics[298] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 576, 448);
    backgroundGraphics[299] = Drawable_newFromSpriteAndSize(landscapeSprites[1], 32, 32, 608, 448);

}

static void createCursor()
{
    cursorSheet = SpriteSheet_new("5_res/cursor.png", NULL);
    cursorSprite = SpriteSheet_getSprite(cursorSheet, 0);
    cursor = Drawable_newFromSpriteAndSize(cursorSprite, 32, 32, 0, 0);
}

static void moveCursor(int keyid)
{
    switch (keyid)
    {
        case BUTTON_UP:
            if (Drawable_getY(cursor) >= 32)
            {
                Drawable_setPos(cursor, Drawable_getX(cursor), Drawable_getY(cursor) - 32);
            }
            break;
        case BUTTON_DOWN:
            if (Drawable_getY(cursor) <= (480 - 64))
            {
                Drawable_setPos(cursor, Drawable_getX(cursor), Drawable_getY(cursor) + 32);
            }
            break;
        case BUTTON_LEFT:
            if (Drawable_getX(cursor) >= 32)
            {
                Drawable_setPos(cursor, Drawable_getX(cursor) - 32, Drawable_getY(cursor));
            }
            break;
        case BUTTON_RIGHT:
            if (Drawable_getX(cursor) <= (640 - 64))
            {
                Drawable_setPos(cursor, Drawable_getX(cursor) + 32, Drawable_getY(cursor));
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

static void _buttonCallback(Controller* controller, int buttonIndex, int state)
{
    smug_assert(controller == theController);
    if (state == SMUG_KEY_RELEASE)
    {
        return;
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
        default:
            smug_assert(FALSE);
    }
}

static void init()
{
    glInit();
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

    Input_initialize();
    theController = Controller_new(0, 6, 1);
    Controller_setButtonCallback(theController, _buttonCallback);
    Controller_setPointerCallback(theController, _pointerCallback);
    Input_linkControllerToKeyboardKey(theController, BUTTON_UP, GLFW_KEY_UP);
    Input_linkControllerToKeyboardKey(theController, BUTTON_DOWN, GLFW_KEY_DOWN);
    Input_linkControllerToKeyboardKey(theController, BUTTON_LEFT, GLFW_KEY_LEFT);
    Input_linkControllerToKeyboardKey(theController, BUTTON_RIGHT, GLFW_KEY_RIGHT);
    Input_linkControllerToKeyboardKey(theController, BUTTON_ENABLE_MOUSE, GLFW_KEY_HOME);
    Input_linkControllerToKeyboardKey(theController, BUTTON_DISABLE_MOUSE, GLFW_KEY_END);
    useMouse(TRUE);

    renderQueue = RenderQueue_new();

    createBackground();
    createCursor();
}

static void runMainLoop()
{
    BOOL running = TRUE;
    while (running)
    {
        // OpenGL rendering goes here...
        glClear(GL_COLOR_BUFFER_BIT);

        drawStuff(renderQueue);
        RenderQueue_render(renderQueue);

        // Swap front and back rendering buffers
        glfwSwapBuffers();

        afterDrawing();

        // Check if ESC key was pressed or window was closed
        running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
    }
}

static void deinit()
{
    RenderQueue_delete(renderQueue);

    for (int i = 0; i < BG_COUNT; i++)
    {
        Drawable_delete(backgroundGraphics[i]);
    }

    for (int i = 0; i < BUILDINGS_COUNT; i++)
    {
        Drawable_delete(buildings[i]);
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

    glfwTerminate();
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
