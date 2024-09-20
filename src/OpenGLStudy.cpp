#include "Demos/DemoHelloTriangle.h";
#include "OpenGL/OpenGL.h"

int main()
{
    DemoHelloTriangle* demoHelloTriangle = new DemoHelloTriangle();
    if(demoHelloTriangle->Init())
    {
        demoHelloTriangle->Start();
    }

    return 0;
}