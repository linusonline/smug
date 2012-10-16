#include <GL/glfw.h>

#include <common.h>
#include <utils/log.h>
#include <graphics/renderqueue.h>
#include <graphics/camera.h>

#include <graphics/graphics.h>

static int windowWidth;
static int windowHeight;

static void (*windowResizeCallback)(int w, int h) = NULL;

static BOOL useWindowCoordinates = TRUE;
static float pixelsPerUnitX = 1;
static float pixelsPerUnitY = 1;
static int originInWindowX = 0;
static int originInWindowY = 0;

static Camera* camera = NULL;
static BOOL userCamera = FALSE;

static BOOL fullscreen = FALSE;

static BOOL isInitialized = FALSE;

static void _adjustCoordinateSystemToWindow(int width, int height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLdouble left = -originInWindowX / pixelsPerUnitX;
    GLdouble top = -originInWindowY / pixelsPerUnitY;
    gluOrtho2D(left, left + width / pixelsPerUnitX, top + height / pixelsPerUnitY, top);
    glMatrixMode(GL_MODELVIEW);
}

static void GLFWCALL _windowResize(int width, int height)
{
    windowWidth = width;
    windowHeight = height;
    DEBUG("Got resize event (%i x %i)", width, height);
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    if (!useWindowCoordinates)
    {
        _adjustCoordinateSystemToWindow(width, height);
    }
    if (windowResizeCallback != NULL)
    {
        windowResizeCallback(width, height);
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
    _adjustCoordinateSystemToWindow(windowWidth, windowHeight);
}

static void openGlfwWindow(int width, int height, BOOL fs)
{
    // int glfwOpenWindow( int width, int height,
    //      int redbits, int greenbits, int bluebits,
    //      int alphabits, int depthbits, int stencilbits,
    //      int mode )
    glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
    if (glfwOpenWindow(width, height, 0, 0, 0, 0, 0, 0, (fs ? GLFW_FULLSCREEN : GLFW_WINDOW)) != GL_TRUE)
    {
        ERROR("Could not open GLFW window! Terminating!");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetWindowSizeCallback(_windowResize);
    glClearColor(0.0, 0.0, 0.0, 0.0);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Graphics_initialize(int width, int height, BOOL fs)
{
    smug_assert(!isInitialized);

    fullscreen = fs;
    if (glfwInit() != GL_TRUE)
    {
        ERROR("Could not initialize GLFW! Terminating!");
        exit(EXIT_FAILURE);
    }

    openGlfwWindow(width, height, fullscreen);

    camera = Camera_new();

    isInitialized = TRUE;
}

void Graphics_terminate()
{
    smug_assert(isInitialized);

    if (!userCamera)
    {
        Camera_delete(camera);
    }
    glfwCloseWindow();
    glfwTerminate();

    isInitialized = FALSE;
}

BOOL Graphics_isInitialized()
{
    return isInitialized;
}

void Graphics_setBackgroundColor(float red, float green, float blue)
{
    glClearColor(red, green, blue, 0.0);
}

void Graphics_setCoordinateSystemForWindow(float left, float top, float width, float height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(left, left + width, top + height, top);
    glMatrixMode(GL_MODELVIEW);
    useWindowCoordinates = TRUE;
}

void Graphics_useCamera(Camera* newCamera)
{
    if (!userCamera)
    {
        Camera_delete(camera);
    }
    camera = newCamera;
    userCamera = TRUE;
}

void Graphics_setWindowResizeCallback(void (*callback)(int w, int h))
{
    windowResizeCallback = callback;
}

void Graphics_render(RenderQueue* renderQueue)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    glTranslatef(-camera->posX, -camera->posY, 0.0);

    RenderQueue_render(renderQueue);

    glPopMatrix();
}

void Graphics_refreshWindow()
{
    glfwSwapBuffers();
}
