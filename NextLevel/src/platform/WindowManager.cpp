#include "WindowManager.h"


#if PLATFORM_WINDOWS
#include "windows/WindowsWindow.h"
#else

#endif

namespace NextLevel
{
#if PLATFORM_WINDOWS
	//! インスタンス。
	HINSTANCE WindowManager::m_hInstance;
#else

#endif

	std::shared_ptr<IWindow> WindowManager::CreateWindowInstanceForPlatform()
	{
#if PLATFORM_WINDOWS
		return std::make_shared<WindowsWindow>(m_hInstance);
#else

#endif

	}
}