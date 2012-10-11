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

static Drawable** world;
static Drawable* cursor;

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
    for (int i = 0; i < TOTAL_OBJECT_COUNT; i++)
    {
        RenderQueue_addDrawable(rq, world[i]);
    }
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
    landscapeSheet = SpriteSheet_new("res/landscape.png", "res/landscape.txt");
    buildingsSheet = SpriteSheet_new("res/buildings.png", "res/buildings.txt");

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

    world = allocatev(Drawable*, TOTAL_OBJECT_COUNT);

    world[0] =  Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 0, 0);
    world[1] =  Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 32, 0);
    world[2] =  Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 64, 0);
    world[3] =  Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 96, 0);
    world[4] =  Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 128, 0);
    world[5] =  Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 160, 0);
    world[6] =  Drawable_newFromSpriteAndDimensions(landscapeSprites[4], 32, 32, 192, 0);
    world[7] =  Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 224, 0);
    world[8] =  Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 256, 0);
    world[9] =  Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 288, 0);
    world[10] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 320, 0);
    world[11] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 352, 0);
    world[12] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 384, 0);
    world[13] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 416, 0);
    world[14] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 448, 0);
    world[15] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 480, 0);
    world[16] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 512, 0);
    world[17] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 544, 0);
    world[18] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 576, 0);
    world[19] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 608, 0);

    world[20] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 0, 32);
    world[21] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 32, 32);
    world[22] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 64, 32);
    world[23] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 96, 32);
    world[24] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 128, 32);
    world[25] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 160, 32);
    world[26] = Drawable_newFromSpriteAndDimensions(landscapeSprites[7], 32, 32, 192, 32);
    world[27] = Drawable_newFromSpriteAndDimensions(landscapeSprites[6], 32, 32, 224, 32);
    world[28] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 256, 32);
    world[29] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 288, 32);
    world[30] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 320, 32);
    world[31] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 352, 32);
    world[32] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 384, 32);
    world[33] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 416, 32);
    world[34] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 448, 32);
    world[35] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 480, 32);
    world[36] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 512, 32);
    world[37] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 544, 32);
    world[38] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 576, 32);
    world[39] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 608, 32);

    world[40] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 0, 64);
    world[41] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 32, 64);
    world[42] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 64, 64);
    world[43] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 96, 64);
    world[44] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 128, 64);
    world[45] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 160, 64);
    world[46] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 192, 64);
    world[47] = Drawable_newFromSpriteAndDimensions(landscapeSprites[7], 32, 32, 224, 64);
    world[48] = Drawable_newFromSpriteAndDimensions(landscapeSprites[3], 32, 32, 256, 64);
    world[49] = Drawable_newFromSpriteAndDimensions(landscapeSprites[9], 32, 32, 288, 64);
    world[50] = Drawable_newFromSpriteAndDimensions(landscapeSprites[3], 32, 32, 320, 64);
    world[51] = Drawable_newFromSpriteAndDimensions(landscapeSprites[3], 32, 32, 352, 64);
    world[52] = Drawable_newFromSpriteAndDimensions(landscapeSprites[3], 32, 32, 384, 64);
    world[53] = Drawable_newFromSpriteAndDimensions(landscapeSprites[3], 32, 32, 416, 64);
    world[54] = Drawable_newFromSpriteAndDimensions(landscapeSprites[3], 32, 32, 448, 64);
    world[55] = Drawable_newFromSpriteAndDimensions(landscapeSprites[3], 32, 32, 480, 64);
    world[56] = Drawable_newFromSpriteAndDimensions(landscapeSprites[3], 32, 32, 512, 64);
    world[57] = Drawable_newFromSpriteAndDimensions(landscapeSprites[3], 32, 32, 544, 64);
    world[58] = Drawable_newFromSpriteAndDimensions(landscapeSprites[3], 32, 32, 576, 64);
    world[59] = Drawable_newFromSpriteAndDimensions(landscapeSprites[3], 32, 32, 608, 64);

    world[60] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 0, 96);
    world[61] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 32, 96);
    world[62] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 64, 96);
    world[63] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 96, 96);
    world[64] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 128, 96);
    world[65] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 160, 96);
    world[66] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 192, 96);
    world[67] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 224, 96);
    world[68] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 256, 96);
    world[69] = Drawable_newFromSpriteAndDimensions(landscapeSprites[21], 32, 32, 288, 96);
    world[70] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 320, 96);
    world[71] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 352, 96);
    world[72] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 384, 96);
    world[73] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 416, 96);
    world[74] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 448, 96);
    world[75] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 480, 96);
    world[76] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 512, 96);
    world[77] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 544, 96);
    world[78] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 576, 96);
    world[79] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 608, 96);

    world[80] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 0, 128);
    world[81] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 32, 128);
    world[82] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 64, 128);
    world[83] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 96, 128);
    world[84] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 128, 128);
    world[85] = Drawable_newFromSpriteAndDimensions(landscapeSprites[2], 32, 32, 160, 128);
    world[86] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 192, 128);
    world[87] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 224, 128);
    world[88] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 256, 128);
    world[89] = Drawable_newFromSpriteAndDimensions(landscapeSprites[4], 32, 32, 288, 128);
    world[90] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 320, 128);
    world[91] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 352, 128);
    world[92] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 384, 128);
    world[93] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 416, 128);
    world[94] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 448, 128);
    world[95] = Drawable_newFromSpriteAndDimensions(landscapeSprites[2], 32, 32, 480, 128);
    world[96] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 512, 128);
    world[97] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 544, 128);
    world[98] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 576, 128);
    world[99] = Drawable_newFromSpriteAndDimensions(landscapeSprites[2], 32, 32, 608, 128);

    world[100] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 0, 160);
    world[101] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 32, 160);
    world[102] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 64, 160);
    world[103] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 96, 160);
    world[104] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 128, 160);
    world[105] = Drawable_newFromSpriteAndDimensions(landscapeSprites[2], 32, 32, 160, 160);
    world[106] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 192, 160);
    world[107] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 224, 160);
    world[108] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 256, 160);
    world[109] = Drawable_newFromSpriteAndDimensions(landscapeSprites[7], 32, 32, 288, 160);
    world[110] = Drawable_newFromSpriteAndDimensions(landscapeSprites[9], 32, 32, 320, 160);
    world[111] = Drawable_newFromSpriteAndDimensions(landscapeSprites[3], 32, 32, 352, 160);
    world[112] = Drawable_newFromSpriteAndDimensions(landscapeSprites[3], 32, 32, 384, 160);
    world[113] = Drawable_newFromSpriteAndDimensions(landscapeSprites[3], 32, 32, 416, 160);
    world[114] = Drawable_newFromSpriteAndDimensions(landscapeSprites[3], 32, 32, 448, 160);
    world[115] = Drawable_newFromSpriteAndDimensions(landscapeSprites[3], 32, 32, 480, 160);
    world[116] = Drawable_newFromSpriteAndDimensions(landscapeSprites[3], 32, 32, 512, 160);
    world[117] = Drawable_newFromSpriteAndDimensions(landscapeSprites[3], 32, 32, 544, 160);
    world[118] = Drawable_newFromSpriteAndDimensions(landscapeSprites[3], 32, 32, 576, 160);
    world[119] = Drawable_newFromSpriteAndDimensions(landscapeSprites[3], 32, 32, 608, 160);

    world[120] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 0, 192);
    world[121] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 32, 192);
    world[122] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 64, 192);
    world[123] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 96, 192);
    world[124] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 128, 192);
    world[125] = Drawable_newFromSpriteAndDimensions(landscapeSprites[2], 32, 32, 160, 192);
    world[126] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 192, 192);
    world[127] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 224, 192);
    world[128] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 256, 192);
    world[129] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 288, 192);
    world[130] = Drawable_newFromSpriteAndDimensions(landscapeSprites[4], 32, 32, 320, 192);
    world[131] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 352, 192);
    world[132] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 384, 192);
    world[133] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 416, 192);
    world[134] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 448, 192);
    world[135] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 480, 192);
    world[136] = Drawable_newFromSpriteAndDimensions(landscapeSprites[2], 32, 32, 512, 192);
    world[137] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 544, 192);
    world[138] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 576, 192);
    world[139] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 608, 192);

    world[140] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 0, 224);
    world[141] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 32, 224);
    world[142] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 64, 224);
    world[143] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 96, 224);
    world[144] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 128, 224);
    world[145] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 160, 224);
    world[146] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 192, 224);
    world[147] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 224, 224);
    world[148] = Drawable_newFromSpriteAndDimensions(landscapeSprites[5], 32, 32, 256, 224);
    world[149] = Drawable_newFromSpriteAndDimensions(landscapeSprites[3], 32, 32, 288, 224);
    world[150] = Drawable_newFromSpriteAndDimensions(landscapeSprites[8], 32, 32, 320, 224);
    world[151] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 352, 224);
    world[152] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 384, 224);
    world[153] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 416, 224);
    world[154] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 448, 224);
    world[155] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 480, 224);
    world[156] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 512, 224);
    world[157] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 544, 224);
    world[158] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 576, 224);
    world[159] = Drawable_newFromSpriteAndDimensions(landscapeSprites[2], 32, 32, 608, 224);

    world[160] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 0, 256);
    world[161] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 32, 256);
    world[162] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 64, 256);
    world[163] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 96, 256);
    world[164] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 128, 256);
    world[165] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 160, 256);
    world[166] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 192, 256);
    world[167] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 224, 256);
    world[168] = Drawable_newFromSpriteAndDimensions(landscapeSprites[4], 32, 32, 256, 256);
    world[169] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 288, 256);
    world[170] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 320, 256);
    world[171] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 352, 256);
    world[172] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 384, 256);
    world[173] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 416, 256);
    world[174] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 448, 256);
    world[175] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 480, 256);
    world[176] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 512, 256);
    world[177] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 544, 256);
    world[178] = Drawable_newFromSpriteAndDimensions(landscapeSprites[2], 32, 32, 576, 256);
    world[179] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 608, 256);

    world[180] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 0, 288);
    world[181] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 32, 288);
    world[182] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 64, 288);
    world[183] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 96, 288);
    world[184] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 128, 288);
    world[185] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 160, 288);
    world[186] = Drawable_newFromSpriteAndDimensions(landscapeSprites[5], 32, 32, 192, 288);
    world[187] = Drawable_newFromSpriteAndDimensions(landscapeSprites[3], 32, 32, 224, 288);
    world[188] = Drawable_newFromSpriteAndDimensions(landscapeSprites[8], 32, 32, 256, 288);
    world[189] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 288, 288);
    world[190] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 320, 288);
    world[191] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 352, 288);
    world[192] = Drawable_newFromSpriteAndDimensions(landscapeSprites[2], 32, 32, 384, 288);
    world[193] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 416, 288);
    world[194] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 448, 288);
    world[195] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 480, 288);
    world[196] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 512, 288);
    world[197] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 544, 288);
    world[198] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 576, 288);
    world[199] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 608, 288);

    world[200] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 0, 320);
    world[201] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 32, 320);
    world[202] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 64, 320);
    world[203] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 96, 320);
    world[204] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 128, 320);
    world[205] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 160, 320);
    world[206] = Drawable_newFromSpriteAndDimensions(landscapeSprites[4], 32, 32, 192, 320);
    world[207] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 224, 320);
    world[208] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 256, 320);
    world[209] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 288, 320);
    world[210] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 320, 320);
    world[211] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 352, 320);
    world[212] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 384, 320);
    world[213] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 416, 320);
    world[214] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 448, 320);
    world[215] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 480, 320);
    world[216] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 512, 320);
    world[217] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 544, 320);
    world[218] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 576, 320);
    world[219] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 608, 320);

    world[220] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 0, 352);
    world[221] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 32, 352);
    world[222] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 64, 352);
    world[223] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 96, 352);
    world[224] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 128, 352);
    world[225] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 160, 352);
    world[226] = Drawable_newFromSpriteAndDimensions(landscapeSprites[4], 32, 32, 192, 352);
    world[227] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 224, 352);
    world[228] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 256, 352);
    world[229] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 288, 352);
    world[230] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 320, 352);
    world[231] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 352, 352);
    world[232] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 384, 352);
    world[233] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 416, 352);
    world[234] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 448, 352);
    world[235] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 480, 352);
    world[236] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 512, 352);
    world[237] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 544, 352);
    world[238] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 576, 352);
    world[239] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 608, 352);

    world[240] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 0, 384);
    world[241] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 32, 384);
    world[242] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 64, 384);
    world[243] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 96, 384);
    world[244] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 128, 384);
    world[245] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 160, 384);
    world[246] = Drawable_newFromSpriteAndDimensions(landscapeSprites[4], 32, 32, 192, 384);
    world[247] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 224, 384);
    world[248] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 256, 384);
    world[249] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 288, 384);
    world[250] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 320, 384);
    world[251] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 352, 384);
    world[252] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 384, 384);
    world[253] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 416, 384);
    world[254] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 448, 384);
    world[255] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 480, 384);
    world[256] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 512, 384);
    world[257] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 544, 384);
    world[258] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 576, 384);
    world[259] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 608, 384);

    world[260] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 0, 416);
    world[261] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 32, 416);
    world[262] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 64, 416);
    world[263] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 96, 416);
    world[264] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 128, 416);
    world[265] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 160, 416);
    world[266] = Drawable_newFromSpriteAndDimensions(landscapeSprites[4], 32, 32, 192, 416);
    world[267] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 224, 416);
    world[268] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 256, 416);
    world[269] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 288, 416);
    world[270] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 320, 416);
    world[271] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 352, 416);
    world[272] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 384, 416);
    world[273] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 416, 416);
    world[274] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 448, 416);
    world[275] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 480, 416);
    world[276] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 512, 416);
    world[277] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 544, 416);
    world[278] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 576, 416);
    world[279] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 608, 416);

    world[280] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 0, 448);
    world[281] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 32, 448);
    world[282] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 64, 448);
    world[283] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 96, 448);
    world[284] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 128, 448);
    world[285] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 160, 448);
    world[286] = Drawable_newFromSpriteAndDimensions(landscapeSprites[4], 32, 32, 192, 448);
    world[287] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 224, 448);
    world[288] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 256, 448);
    world[289] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 288, 448);
    world[290] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 320, 448);
    world[291] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 352, 448);
    world[292] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 384, 448);
    world[293] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 416, 448);
    world[294] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 448, 448);
    world[295] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 480, 448);
    world[296] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 512, 448);
    world[297] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], 32, 32, 544, 448);
    world[298] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 576, 448);
    world[299] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], 32, 32, 608, 448);

    world[300] =  Drawable_newFromSpriteAndDimensions(buildingSprites[45], 40, 64, 288, -32);
    world[301] = Drawable_newFromSpriteAndDimensions(buildingSprites[45], 40, 64, 320, -32);
    world[302] = Drawable_newFromSpriteAndDimensions(buildingSprites[45], 40, 64, 352, -32);
    world[303] = Drawable_newFromSpriteAndDimensions(buildingSprites[45], 40, 64, 384, -32);
    world[304] = Drawable_newFromSpriteAndDimensions(buildingSprites[45], 40, 64, 416, -32);
    world[305] = Drawable_newFromSpriteAndDimensions(buildingSprites[45], 40, 64, 64, 32);
    world[306] = Drawable_newFromSpriteAndDimensions(buildingSprites[3], 40, 64, 64, 160);
    world[307] = Drawable_newFromSpriteAndDimensions(buildingSprites[5], 40, 64, 128, 160);
    world[308] = Drawable_newFromSpriteAndDimensions(buildingSprites[3], 40, 64, 32, 192);
    world[309] = Drawable_newFromSpriteAndDimensions(buildingSprites[0], 40, 64, 64, 192);
    world[310] = Drawable_newFromSpriteAndDimensions(buildingSprites[3], 40, 64, 96, 192);
    world[311] = Drawable_newFromSpriteAndDimensions(buildingSprites[3], 40, 64, 64, 224);

    animHouse = SpriteAnimation_newEmpty();
    SpriteAnimation_addFrame(animHouse, buildingSprites[10], 0.5);
    SpriteAnimation_addFrame(animHouse, buildingSprites[11], 0.5);
    world[312] = Drawable_newFromSpriteAnimationAndDimensions(animHouse, 40, 64, 128, 192);
    SpriteAnimation_start(animHouse);
}

static void createCursor()
{
    cursorSheet = SpriteSheet_new("res/cursor.png", NULL);
    cursorSprite = SpriteSheet_getSprite(cursorSheet, 0);
    cursor = Drawable_newFromSpriteAndDimensions(cursorSprite, 32, 32, 0, 0);
    world[313] = cursor;
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

static void _pointerCallback(Controller* controller, int pointerIndex, int xDelta, int yDelta)
{
    DEBUG("Pointer moved by %i, %i", xDelta, yDelta);
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
    Log_setLevel(LOG_ALL);
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

    for (int i = 0; i < TOTAL_OBJECT_COUNT; i++)
    {
        Drawable_delete(world[i]);
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
