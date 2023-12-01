
#ifndef ___SRC_CORE_ENGINE_H___
#define ___SRC_CORE_ENGINE_H___

#include <string>
#include <memory>

#include "Window.h"

class Engine
{
public:
	Engine();
	~Engine();
	void Run();
private:
	void DoFrame(float dt);
private:
	Window m_window;

	DWORD m_dwExecLastTime;
	DWORD m_dwCurrentTime;
	float m_dwFps = 60.0f;
	float m_dwTimeScale = 1.0f;
};

#endif //!___SRC_CORE_ENGINE_H___