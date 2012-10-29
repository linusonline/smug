#include <common.h>
#include <graphics/camera.h>

Camera* Camera_new()
{
    Camera* newCamera = allocate(Camera);
    newCamera->posX = 0.0;
    newCamera->posY = 0.0;
    newCamera->scaleX = 1.0;
    newCamera->scaleY = 1.0;
    newCamera->rotation = 0.0;
    return newCamera;
}

void Camera_delete(void* camera)
{
    smug_assert(NULL != camera);
    free((Camera*)camera);
}
