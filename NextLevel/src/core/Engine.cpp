#include "Engine.h"

#include <timeapi.h>
#pragma comment(lib, "winmm.lib")


Engine::Engine()
	: m_window(1280, 720, "NextLevel")
{
}

Engine::~Engine()
{}

void Engine::Run()
{
	while (1)
	{
		if (!m_window.ProcessMessages())
		{
			break;
		}
		else
		{
			m_dwCurrentTime = timeGetTime();

			DWORD deltaTime = m_dwCurrentTime - m_dwExecLastTime;
			if (deltaTime >= (1000 / (m_dwFps * m_dwTimeScale)))
			{
				DoFrame(deltaTime);
				m_dwExecLastTime = m_dwCurrentTime;
			}
		}
	}
}

void Engine::DoFrame(float dt)
{

}