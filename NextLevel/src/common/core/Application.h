#ifndef ___SRC_CORE_APPLICATION_H___
#define ___SRC_CORE_APPLICATION_H___

#include <string>
#include <memory>

#include "../../platform/WindowBase.h"
#include "utils/Timer.h"

namespace NextLevel
{
	/**
	* @class Application
	* @brief アプリケーションクラス。
	*/
	class Application {
	public:
		/**
		* @brief 初期化処理をします。
		*/
		void Init();

		/**
		* @brief 終了処理をします。
		*/
		void Uninit();

		/**
		* @brief 処理を実行します。
		*/
		void Run();

	private:
		/**
		* @brief 毎フレーム処理を実行します。
		* @param _deltaTime 前フレームからの経過時間。
		*/
		void DoFrame(float _deltaTime);

	private:
		//! ウィンドウインスタンス
		std::shared_ptr<IWindow> m_window;
		//! 最後の実行時間
		unsigned long m_dwExecLastTime;
		//! 現在時間
		unsigned long m_dwCurrentTime;
		//! フレームレート
		float m_fFps = 60.0f;
		//! タイムスケール
		float m_fTimeScale = 1.0f;
	};
}

#endif //!___SRC_CORE_ENGINE_H___