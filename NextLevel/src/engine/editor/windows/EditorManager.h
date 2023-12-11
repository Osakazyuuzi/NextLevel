#ifndef ___SRC_ENGINE_EDITOR_WINDOWS_EDITORMANAGER_H___
#define ___SRC_ENGINE_EDITOR_WINDOWS_EDITORMANAGER_H___

#include <d3d11.h>
#include "src/common/core/utils/Timer.h"

namespace NextLevel
{
	class EditorManager {
	private:
		/**
		* @brief �R���X�g���N�^�B
		*/
		EditorManager() = default;

		/**
		* @brief �f�X�g���N�^�B
		*/
		~EditorManager() = default;

	public:
		/**
		 * @brief �V���O���g���C���X�^���X���擾���܂��B
		 * @return EditorManager& �O���t�B�b�N�X�N���X�̃C���X�^���X�B
		 */
		static EditorManager& GetInstance()
		{
			static EditorManager instance;
			return instance;
		}

		/**
		* @brief �G�f�B�^�̏����������܂��B
		* @param _hWnd �E�B���h�E�n���h���B
		*/
		void Startup(HWND _hWnd);

		/**
		* @brief �G�f�B�^�̏I�����������܂��B
		*/
		void Shutdown();

		/**
		* @brief �G�f�B�^�̕`��O���������܂��B
		*/
		void BeginRender();

		/**
		* @brief �G�f�B�^�̕`��㏈�������܂��B
		*/
		void EndRender();

		/**
		* @brief �G�f�B�^�̕`������܂��B
		*/
		void Draw();

	private:
		bool m_bMaster;
		bool m_bScene;
		bool m_bHierarchy;
		bool m_bInspector;
		bool m_bConsole;
		bool m_bAssets;
		bool m_bDebug = true;
	private:
		Timer m_timer;
	};

}


#endif //!___SRC_ENGINE_EDITOR_WINDOWS_EDITORMANAGER_H___