#ifndef ___SRC_COMMON_PLATFORM_WINDOWBASE_H___
#define ___SRC_COMMON_PLATFORM_WINDOWBASE_H___

#include <string>
#include <optional>

namespace NextLevel
{
	/**
	* @class IWindow
	* @brief ウィンドウクラスの基底クラス。
	*	全てのウィンドウはこのクラスを継承する。
	*/
	class IWindow {
	public:
		/**
		* @brief コンストラクタ。
		*/
		IWindow() = default;

		/**
		* @brief デストラクタ。
		*/
		~IWindow() = default;

		/**
		* @brief ウィンドウを作成します。
		* @param _width ウィンドウの横幅。
		* @param _height ウィンドウの縦幅。
		* @param _title ウィンドウのタイトル名。
		*/
		virtual bool Create(
			int					_width,
			int					_height,
			const std::string&	_title) = 0;

		/**
		* @brief イベントを実行します。
		*/
		virtual bool ProcessEvent() = 0;

		/**
		* @brief ウィンドウの横幅を取得します。
		* @return int ウィンドウの横幅。
		*/
		virtual int GetWidth() const noexcept
		{
			return m_nWidth;
		}

		/**
		* @brief ウィンドウの縦幅を取得します。
		* @return int ウィンドウの縦幅。
		*/
		virtual int GetHeight() const noexcept
		{
			return m_nHeight;
		}

	protected:
		//! ウィンドウの横幅
		int m_nWidth;
		//! ウィンドウの縦幅
		int m_nHeight;
		//! ウィンドウタイトル
		std::string m_szTitle;
	};
}

#endif //!___SRC_COMMON_PLATFORM_WINOW_BASE_H___