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

static int windowWidth;
static int windowHeight;

static BOOL useWindowCoordinates = TRUE;
static float pixelsPerUnitX = 1;
static float pixelsPerUnitY = 1;
static int originInWindowX = 0;
static int originInWindowY = 0;

static int cursorPositionX;
static int cursorPositionY;
static const int U = 32; // Unit size
static const int WORLD_WIDTH = 640;     // This world happens to be just one screen big.
static const int WORLD_HEIGHT = 480;

static void adjustCoordinateSystemToWindow(int width, int height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLdouble left = -originInWindowX / pixelsPerUnitX;
    GLdouble top = -originInWindowY / pixelsPerUnitY;
    gluOrtho2D(left, left + width / pixelsPerUnitX, top + height / pixelsPerUnitY, top);
    glMatrixMode(GL_MODELVIEW);
}

static void GLFWCALL windowResize(int width, int height)
{
    windowWidth = width;
    windowHeight = height;
    DEBUG("Got resize event (%i x %i)", width, height);
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    if (!useWindowCoordinates)
    {
        adjustCoordinateSystemToWindow(width, height);
    }
}

// @param originInWindowX/Y Origin of the coordinate system relative to top left corner of the window. Specified in pixels.
static void setCoordinateSystemInPixelsPerUnit(float ppux, float ppuy, int originX, int originY)
{
    pixelsPerUnitX = ppux;
    pixelsPerUnitY = ppuy;
    originInWindowX = originX;
    originInWindowY = originY;
    useWindowCoordinates = FALSE;
    adjustCoordinateSystemToWindow(windowWidth, windowHeight);
}

static void setCoordinateSystemForWindow(float left, float top, float width, float height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(left, left + width, top + height, top);
    glMatrixMode(GL_MODELVIEW);
    useWindowCoordinates = TRUE;
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

    world[0] =  Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, 0, 0);
    world[1] =  Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U, 0);
    world[2] =  Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*2, 0);
    world[3] =  Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*3, 0);
    world[4] =  Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*4, 0);
    world[5] =  Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*5, 0);
    world[6] =  Drawable_newFromSpriteAndDimensions(landscapeSprites[4], U, U, U*6, 0);
    world[7] =  Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*7, 0);
    world[8] =  Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*8, 0);
    world[9] =  Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*9, 0);
    world[10] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*10, 0);
    world[11] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*11, 0);
    world[12] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*12, 0);
    world[13] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*13, 0);
    world[14] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*14, 0);
    world[15] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*15, 0);
    world[16] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*16, 0);
    world[17] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*17, 0);
    world[18] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*18, 0);
    world[19] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*19, 0);

    world[20] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, 0, U);
    world[21] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U, U);
    world[22] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*2, U);
    world[23] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*3, U);
    world[24] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*4, U);
    world[25] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*5, U);
    world[26] = Drawable_newFromSpriteAndDimensions(landscapeSprites[7], U, U, U*6, U);
    world[27] = Drawable_newFromSpriteAndDimensions(landscapeSprites[6], U, U, U*7, U);
    world[28] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*8, U);
    world[29] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*9, U);
    world[30] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*10, U);
    world[31] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*11, U);
    world[32] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*12, U);
    world[33] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*13, U);
    world[34] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*14, U);
    world[35] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*15, U);
    world[36] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*16, U);
    world[37] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*17, U);
    world[38] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*18, U);
    world[39] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*19, U);

    world[40] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, 0, U*2);
    world[41] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U, U*2);
    world[42] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*2, U*2);
    world[43] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*3, U*2);
    world[44] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*4, U*2);
    world[45] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*5, U*2);
    world[46] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*6, U*2);
    world[47] = Drawable_newFromSpriteAndDimensions(landscapeSprites[7], U, U, U*7, U*2);
    world[48] = Drawable_newFromSpriteAndDimensions(landscapeSprites[3], U, U, U*8, U*2);
    world[49] = Drawable_newFromSpriteAndDimensions(landscapeSprites[9], U, U, U*9, U*2);
    world[50] = Drawable_newFromSpriteAndDimensions(landscapeSprites[3], U, U, U*10, U*2);
    world[51] = Drawable_newFromSpriteAndDimensions(landscapeSprites[3], U, U, U*11, U*2);
    world[52] = Drawable_newFromSpriteAndDimensions(landscapeSprites[3], U, U, U*12, U*2);
    world[53] = Drawable_newFromSpriteAndDimensions(landscapeSprites[3], U, U, U*13, U*2);
    world[54] = Drawable_newFromSpriteAndDimensions(landscapeSprites[3], U, U, U*14, U*2);
    world[55] = Drawable_newFromSpriteAndDimensions(landscapeSprites[3], U, U, U*15, U*2);
    world[56] = Drawable_newFromSpriteAndDimensions(landscapeSprites[3], U, U, U*16, U*2);
    world[57] = Drawable_newFromSpriteAndDimensions(landscapeSprites[3], U, U, U*17, U*2);
    world[58] = Drawable_newFromSpriteAndDimensions(landscapeSprites[3], U, U, U*18, U*2);
    world[59] = Drawable_newFromSpriteAndDimensions(landscapeSprites[3], U, U, U*19, U*2);

    world[60] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, 0, U*3);
    world[61] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U, U*3);
    world[62] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*2, U*3);
    world[63] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*3, U*3);
    world[64] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*4, U*3);
    world[65] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*5, U*3);
    world[66] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*6, U*3);
    world[67] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*7, U*3);
    world[68] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*8, U*3);
    world[69] = Drawable_newFromSpriteAndDimensions(landscapeSprites[21], U, U, U*9, U*3);
    world[70] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*10, U*3);
    world[71] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*11, U*3);
    world[72] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*12, U*3);
    world[73] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*13, U*3);
    world[74] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*14, U*3);
    world[75] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*15, U*3);
    world[76] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*16, U*3);
    world[77] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*17, U*3);
    world[78] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*18, U*3);
    world[79] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*19, U*3);

    world[80] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, 0, U*4);
    world[81] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U, U*4);
    world[82] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*2, U*4);
    world[83] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*3, U*4);
    world[84] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*4, U*4);
    world[85] = Drawable_newFromSpriteAndDimensions(landscapeSprites[2], U, U, U*5, U*4);
    world[86] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*6, U*4);
    world[87] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*7, U*4);
    world[88] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*8, U*4);
    world[89] = Drawable_newFromSpriteAndDimensions(landscapeSprites[4], U, U, U*9, U*4);
    world[90] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*10, U*4);
    world[91] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*11, U*4);
    world[92] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*12, U*4);
    world[93] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*13, U*4);
    world[94] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*14, U*4);
    world[95] = Drawable_newFromSpriteAndDimensions(landscapeSprites[2], U, U, U*15, U*4);
    world[96] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*16, U*4);
    world[97] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*17, U*4);
    world[98] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*18, U*4);
    world[99] = Drawable_newFromSpriteAndDimensions(landscapeSprites[2], U, U, U*19, U*4);

    world[100] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, 0, U*5);
    world[101] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U, U*5);
    world[102] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*2, U*5);
    world[103] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*3, U*5);
    world[104] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*4, U*5);
    world[105] = Drawable_newFromSpriteAndDimensions(landscapeSprites[2], U, U, U*5, U*5);
    world[106] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*6, U*5);
    world[107] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*7, U*5);
    world[108] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*8, U*5);
    world[109] = Drawable_newFromSpriteAndDimensions(landscapeSprites[7], U, U, U*9, U*5);
    world[110] = Drawable_newFromSpriteAndDimensions(landscapeSprites[9], U, U, U*10, U*5);
    world[111] = Drawable_newFromSpriteAndDimensions(landscapeSprites[3], U, U, U*11, U*5);
    world[112] = Drawable_newFromSpriteAndDimensions(landscapeSprites[3], U, U, U*12, U*5);
    world[113] = Drawable_newFromSpriteAndDimensions(landscapeSprites[3], U, U, U*13, U*5);
    world[114] = Drawable_newFromSpriteAndDimensions(landscapeSprites[3], U, U, U*14, U*5);
    world[115] = Drawable_newFromSpriteAndDimensions(landscapeSprites[3], U, U, U*15, U*5);
    world[116] = Drawable_newFromSpriteAndDimensions(landscapeSprites[3], U, U, U*16, U*5);
    world[117] = Drawable_newFromSpriteAndDimensions(landscapeSprites[3], U, U, U*17, U*5);
    world[118] = Drawable_newFromSpriteAndDimensions(landscapeSprites[3], U, U, U*18, U*5);
    world[119] = Drawable_newFromSpriteAndDimensions(landscapeSprites[3], U, U, U*19, U*5);

    world[120] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, 0, U*6);
    world[121] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U, U*6);
    world[122] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*2, U*6);
    world[123] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*3, U*6);
    world[124] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*4, U*6);
    world[125] = Drawable_newFromSpriteAndDimensions(landscapeSprites[2], U, U, U*5, U*6);
    world[126] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*6, U*6);
    world[127] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*7, U*6);
    world[128] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*8, U*6);
    world[129] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*9, U*6);
    world[130] = Drawable_newFromSpriteAndDimensions(landscapeSprites[4], U, U, U*10, U*6);
    world[131] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*11, U*6);
    world[132] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*12, U*6);
    world[133] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*13, U*6);
    world[134] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*14, U*6);
    world[135] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*15, U*6);
    world[136] = Drawable_newFromSpriteAndDimensions(landscapeSprites[2], U, U, U*16, U*6);
    world[137] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*17, U*6);
    world[138] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*18, U*6);
    world[139] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*19, U*6);

    world[140] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, 0, U*7);
    world[141] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U, U*7);
    world[142] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*2, U*7);
    world[143] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*3, U*7);
    world[144] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*4, U*7);
    world[145] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*5, U*7);
    world[146] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*6, U*7);
    world[147] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*7, U*7);
    world[148] = Drawable_newFromSpriteAndDimensions(landscapeSprites[5], U, U, U*8, U*7);
    world[149] = Drawable_newFromSpriteAndDimensions(landscapeSprites[3], U, U, U*9, U*7);
    world[150] = Drawable_newFromSpriteAndDimensions(landscapeSprites[8], U, U, U*10, U*7);
    world[151] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*11, U*7);
    world[152] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*12, U*7);
    world[153] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*13, U*7);
    world[154] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*14, U*7);
    world[155] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*15, U*7);
    world[156] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*16, U*7);
    world[157] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*17, U*7);
    world[158] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*18, U*7);
    world[159] = Drawable_newFromSpriteAndDimensions(landscapeSprites[2], U, U, U*19, U*7);

    world[160] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, 0, U*8);
    world[161] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U, U*8);
    world[162] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*2, U*8);
    world[163] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*3, U*8);
    world[164] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*4, U*8);
    world[165] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*5, U*8);
    world[166] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*6, U*8);
    world[167] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*7, U*8);
    world[168] = Drawable_newFromSpriteAndDimensions(landscapeSprites[4], U, U, U*8, U*8);
    world[169] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*9, U*8);
    world[170] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*10, U*8);
    world[171] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*11, U*8);
    world[172] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*12, U*8);
    world[173] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*13, U*8);
    world[174] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*14, U*8);
    world[175] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*15, U*8);
    world[176] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*16, U*8);
    world[177] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*17, U*8);
    world[178] = Drawable_newFromSpriteAndDimensions(landscapeSprites[2], U, U, U*18, U*8);
    world[179] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*19, U*8);

    world[180] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, 0, U*9);
    world[181] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U, U*9);
    world[182] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*2, U*9);
    world[183] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*3, U*9);
    world[184] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*4, U*9);
    world[185] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*5, U*9);
    world[186] = Drawable_newFromSpriteAndDimensions(landscapeSprites[5], U, U, U*6, U*9);
    world[187] = Drawable_newFromSpriteAndDimensions(landscapeSprites[3], U, U, U*7, U*9);
    world[188] = Drawable_newFromSpriteAndDimensions(landscapeSprites[8], U, U, U*8, U*9);
    world[189] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*9, U*9);
    world[190] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*10, U*9);
    world[191] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*11, U*9);
    world[192] = Drawable_newFromSpriteAndDimensions(landscapeSprites[2], U, U, U*12, U*9);
    world[193] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*13, U*9);
    world[194] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*14, U*9);
    world[195] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*15, U*9);
    world[196] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*16, U*9);
    world[197] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*17, U*9);
    world[198] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*18, U*9);
    world[199] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*19, U*9);

    world[200] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, 0, U*10);
    world[201] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U, U*10);
    world[202] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*2, U*10);
    world[203] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*3, U*10);
    world[204] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*4, U*10);
    world[205] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*5, U*10);
    world[206] = Drawable_newFromSpriteAndDimensions(landscapeSprites[4], U, U, U*6, U*10);
    world[207] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*7, U*10);
    world[208] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*8, U*10);
    world[209] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*9, U*10);
    world[210] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*10, U*10);
    world[211] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*11, U*10);
    world[212] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*12, U*10);
    world[213] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*13, U*10);
    world[214] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*14, U*10);
    world[215] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*15, U*10);
    world[216] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*16, U*10);
    world[217] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*17, U*10);
    world[218] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*18, U*10);
    world[219] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*19, U*10);

    world[220] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, 0, U*11);
    world[221] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U, U*11);
    world[222] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*2, U*11);
    world[223] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*3, U*11);
    world[224] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*4, U*11);
    world[225] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*5, U*11);
    world[226] = Drawable_newFromSpriteAndDimensions(landscapeSprites[4], U, U, U*6, U*11);
    world[227] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*7, U*11);
    world[228] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*8, U*11);
    world[229] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*9, U*11);
    world[230] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*10, U*11);
    world[231] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*11, U*11);
    world[232] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*12, U*11);
    world[233] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*13, U*11);
    world[234] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*14, U*11);
    world[235] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*15, U*11);
    world[236] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*16, U*11);
    world[237] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*17, U*11);
    world[238] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*18, U*11);
    world[239] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*19, U*11);

    world[240] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, 0, U*12);
    world[241] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U, U*12);
    world[242] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*2, U*12);
    world[243] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*3, U*12);
    world[244] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*4, U*12);
    world[245] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*5, U*12);
    world[246] = Drawable_newFromSpriteAndDimensions(landscapeSprites[4], U, U, U*6, U*12);
    world[247] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*7, U*12);
    world[248] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*8, U*12);
    world[249] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*9, U*12);
    world[250] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*10, U*12);
    world[251] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*11, U*12);
    world[252] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*12, U*12);
    world[253] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*13, U*12);
    world[254] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*14, U*12);
    world[255] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*15, U*12);
    world[256] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*16, U*12);
    world[257] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*17, U*12);
    world[258] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*18, U*12);
    world[259] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*19, U*12);

    world[260] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, 0, U*13);
    world[261] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U, U*13);
    world[262] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*2, U*13);
    world[263] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*3, U*13);
    world[264] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*4, U*13);
    world[265] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*5, U*13);
    world[266] = Drawable_newFromSpriteAndDimensions(landscapeSprites[4], U, U, U*6, U*13);
    world[267] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*7, U*13);
    world[268] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*8, U*13);
    world[269] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*9, U*13);
    world[270] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*10, U*13);
    world[271] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*11, U*13);
    world[272] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*12, U*13);
    world[273] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*13, U*13);
    world[274] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*14, U*13);
    world[275] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*15, U*13);
    world[276] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*16, U*13);
    world[277] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*17, U*13);
    world[278] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*18, U*13);
    world[279] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*19, U*13);

    world[280] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, 0, U*14);
    world[281] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U, U*14);
    world[282] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*2, U*14);
    world[283] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*3, U*14);
    world[284] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*4, U*14);
    world[285] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*5, U*14);
    world[286] = Drawable_newFromSpriteAndDimensions(landscapeSprites[4], U, U, U*6, U*14);
    world[287] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*7, U*14);
    world[288] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*8, U*14);
    world[289] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*9, U*14);
    world[290] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*10, U*14);
    world[291] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*11, U*14);
    world[292] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*12, U*14);
    world[293] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*13, U*14);
    world[294] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*14, U*14);
    world[295] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*15, U*14);
    world[296] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*16, U*14);
    world[297] = Drawable_newFromSpriteAndDimensions(landscapeSprites[0], U, U, U*17, U*14);
    world[298] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*18, U*14);
    world[299] = Drawable_newFromSpriteAndDimensions(landscapeSprites[1], U, U, U*19, U*14);

    world[300] =  Drawable_newFromSpriteAndDimensions(buildingSprites[45], 40, 64, U*9, -U);
    world[301] = Drawable_newFromSpriteAndDimensions(buildingSprites[45], 40, 64, U*10, -U);
    world[302] = Drawable_newFromSpriteAndDimensions(buildingSprites[45], 40, 64, U*11, -U);
    world[303] = Drawable_newFromSpriteAndDimensions(buildingSprites[45], 40, 64, U*12, -U);
    world[304] = Drawable_newFromSpriteAndDimensions(buildingSprites[45], 40, 64, U*13, -U);
    world[305] = Drawable_newFromSpriteAndDimensions(buildingSprites[45], 40, 64, U*2, U);
    world[306] = Drawable_newFromSpriteAndDimensions(buildingSprites[3], 40, 64, U*2, U*5);
    world[307] = Drawable_newFromSpriteAndDimensions(buildingSprites[5], 40, 64, U*4, U*5);
    world[308] = Drawable_newFromSpriteAndDimensions(buildingSprites[3], 40, 64, U, U*6);
    world[309] = Drawable_newFromSpriteAndDimensions(buildingSprites[0], 40, 64, U*2, U*6);
    world[310] = Drawable_newFromSpriteAndDimensions(buildingSprites[3], 40, 64, U*3, U*6);
    world[311] = Drawable_newFromSpriteAndDimensions(buildingSprites[3], 40, 64, U*2, U*7);

    animHouse = SpriteAnimation_newEmpty();
    SpriteAnimation_addFrame(animHouse, buildingSprites[10], 0.5);
    SpriteAnimation_addFrame(animHouse, buildingSprites[11], 0.5);
    world[312] = Drawable_newFromSpriteAnimationAndDimensions(animHouse, 40, 64, U*4, U*6);
    SpriteAnimation_start(animHouse);
}

static void createCursor(int posX, int posY)
{
    cursorSheet = SpriteSheet_new("res/cursor.png", NULL);
    cursorSprite = SpriteSheet_getSprite(cursorSheet, 0);
    cursor = Drawable_newFromSpriteAndDimensions(cursorSprite, U, U, posX*U, posY*U);
    world[313] = cursor;
}

static void moveCursor(int keyid)
{
    switch (keyid)
    {
        case BUTTON_UP:
            if (Drawable_getY(cursor) >= U)
            {
                Drawable_setPos(cursor, Drawable_getX(cursor), Drawable_getY(cursor) - U);
                if (abs(Drawable_getY(cursor) - camera->posY) > windowHeight/4)
                {
                    camera->posY -= U;
                }
            }
            break;
        case BUTTON_DOWN:
            if (Drawable_getY(cursor) <= (WORLD_HEIGHT - U*2))
            {
                Drawable_setPos(cursor, Drawable_getX(cursor), Drawable_getY(cursor) + U);
                if (abs(Drawable_getY(cursor) - camera->posY) > windowHeight/4)
                {
                    camera->posY += U;
                }
            }
            break;
        case BUTTON_LEFT:
            if (Drawable_getX(cursor) >= U)
            {
                Drawable_setPos(cursor, Drawable_getX(cursor) - U, Drawable_getY(cursor));
                if (abs(Drawable_getX(cursor) - camera->posX) > windowWidth/4)
                {
                    camera->posX -= U;
                }
            }
            break;
        case BUTTON_RIGHT:
            if (Drawable_getX(cursor) <= (WORLD_WIDTH - U*2))
            {
                Drawable_setPos(cursor, Drawable_getX(cursor) + U, Drawable_getY(cursor));
                if (abs(Drawable_getX(cursor) - camera->posX) > windowWidth/4)
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
        case BUTTON_PAN_UP:
        case BUTTON_PAN_DOWN:
        case BUTTON_PAN_LEFT:
        case BUTTON_PAN_RIGHT:
            SMUG_NOOP(); break;
        default:
            smug_assert(FALSE);
    }
}

static void init()
{
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

    glInit();
    setCoordinateSystemForWindow(-WORLD_WIDTH/2, -WORLD_HEIGHT/2, WORLD_WIDTH, WORLD_HEIGHT);
    // setCoordinateSystemInPixelsPerUnit(1.3, 1.3, 20.0, 20.0);

    Input_initialize();
    theController = Controller_new(0, 10, 1);
    Controller_setButtonCallback(theController, _buttonCallback);
    Controller_setPointerCallback(theController, _pointerCallback);
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
    camera = Camera_new();
    camera->posX = 320;
    camera->posY = 240;

    createBackground();
    cursorPositionX = 10;
    cursorPositionY = 8;
    createCursor(cursorPositionX, cursorPositionY);
}

static void actuallyDrawStuff(RenderQueue* rq)
{
    glPushMatrix();
    glTranslatef(-camera->posX, -camera->posY, 0.0);

    RenderQueue_render(rq);

    glPopMatrix();
}

static void runMainLoop()
{
    BOOL running = TRUE;
    while (running)
    {
        // OpenGL rendering goes here...
        glClear(GL_COLOR_BUFFER_BIT);

        drawStuff(renderQueue);
        actuallyDrawStuff(renderQueue);

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
