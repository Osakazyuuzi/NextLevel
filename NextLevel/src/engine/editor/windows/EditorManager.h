#ifndef ___SRC_ENGINE_EDITOR_WINDOWS_EDITORMANAGER_H___
#define ___SRC_ENGINE_EDITOR_WINDOWS_EDITORMANAGER_H___

#include <d3d11.h>
#include "src/common/core/utils/Timer.h"

namespace NextLevel
{
	class EditorManager {
	private:
		/**
		* @brief コンストラクタ。
		*/
		EditorManager() = default;

		/**
		* @brief デストラクタ。
		*/
		~EditorManager() = default;

	public:
		/**
		 * @brief シングルトンインスタンスを取得します。
		 * @return EditorManager& グラフィックスクラスのインスタンス。
		 */
		static EditorManager& GetInstance()
		{
			static EditorManager instance;
			return instance;
		}

		/**
		* @brief エディタの初期化をします。
		* @param _hWnd ウィンドウハンドル。
		*/
		void Startup(HWND _hWnd);

		/**
		* @brief エディタの終了処理をします。
		*/
		void Shutdown();

		/**
		* @brief エディタの描画前準備をします。
		*/
		void BeginRender();

		/**
		* @brief エディタの描画後処理をします。
		*/
		void EndRender();

		/**
		* @brief エディタの描画をします。
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