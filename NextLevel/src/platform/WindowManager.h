#ifndef ___SRC_COMMON_PLATFORM_WINDOWMANAGER_H___
#define ___SRC_COMMON_PLATFORM_WINDOWMANAGER_H___

#include <memory>
#include "WindowBase.h"
#include "../common/core/PlatformDefinitions.h"

#if PLATFORM_WINDOWS
#include <Windows.h>
#else

#endif

namespace NextLevel
{
	/**
	* @class WindowManager
	* @brief プラットフォームに応じてウィンドウを管理するクラス。
	*/
	class WindowManager {
	public:
		/**
		* @brief プラットフォームに応じてウィンドウインスタンスを作成する。
		* @return std::shared_ptr<IWindow> ウィンドウインスタンス。
		*/
		static std::shared_ptr<IWindow> CreateWindowInstanceForPlatform();
		
#if PLATFORM_WINDOWS
		/**
		* @brief HINSTANCEを設定する。
		* @param _hInstance インスタンス。
		*/
		static void SetHInstance(HINSTANCE _hInstance)
		{
			m_hInstance = _hInstance;
		}

	private:
		//! インスタンス。
		static HINSTANCE m_hInstance;
#else

#endif
	};
}

#endif //!___SRC_COMMON_PLATFORM_WINDOWMANAGER_H___