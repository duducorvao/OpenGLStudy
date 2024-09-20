#pragma once
#include "../OpenGL/OpenGL.h"

class DemoBase
{

#define MAIN_WINDOW m_OpenGL->m_appWindow

protected:
	OpenGL* m_OpenGL;

public:
	DemoBase();

	virtual ~DemoBase() {};

	virtual bool Init() = 0;

	virtual void InitBuffers() = 0;
	virtual void InitShaders() = 0;

	virtual void Start();

	virtual void Release() = 0;

	virtual void Tick();

	virtual void Update();

	virtual void PreDraw();
	virtual void Draw() = 0;
	virtual void PostDraw();

	virtual void ProcessInput() = 0;
};