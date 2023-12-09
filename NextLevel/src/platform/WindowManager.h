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
	* @brief �v���b�g�t�H�[���ɉ����ăE�B���h�E���Ǘ�����N���X�B
	*/
	class WindowManager {
	public:
		/**
		* @brief �v���b�g�t�H�[���ɉ����ăE�B���h�E�C���X�^���X���쐬����B
		* @return std::shared_ptr<IWindow> �E�B���h�E�C���X�^���X�B
		*/
		static std::shared_ptr<IWindow> CreateWindowInstanceForPlatform();
		
#if PLATFORM_WINDOWS
		/**
		* @brief HINSTANCE��ݒ肷��B
		* @param _hInstance �C���X�^���X�B
		*/
		static void SetHInstance(HINSTANCE _hInstance)
		{
			m_hInstance = _hInstance;
		}

	private:
		//! �C���X�^���X�B
		static HINSTANCE m_hInstance;
#else

#endif
	};
}

#endif //!___SRC_COMMON_PLATFORM_WINDOWMANAGER_H___