#include "OpenGL/OpenGL.h"

int main()
{
    OpenGL* openGL = new OpenGL();
    bool isInitialized = openGL->Initialize();
    if (isInitialized)
    {
        openGL->SetViewport(0, 0, 800, 600);
        openGL->Start();
    }

    return 0;
}