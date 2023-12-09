#ifndef ___SRC_COMMON_CORE_UTILS_TIMER_H___
#define ___SRC_COMMON_CORE_UTILS_TIMER_H___

#include <chrono>

namespace NextLevel
{
	/**
	* @class Timer
	* @brief 高精度で時間を計測するクラス。
	*/
	class Timer {
		//! 別名エイリアス
		using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;
	
	public:
		/**
		* @brief コンストラクタ。
		*/
		Timer() 
			: m_start(std::chrono::high_resolution_clock::now())
			, m_prev(m_start)
		{}

		/**
		* @brief 前回時間をリセットします。
		*/
		void Reset()
		{
			m_prev = std::chrono::high_resolution_clock::now();
		}

		/**
		* @brief 前回リセット時間からの経過時間を取得します。
		* @return double 前回リセット時間からの経過時間。
		*/
		double GetDeltaTime()
		{
			TimePoint now = std::chrono::high_resolution_clock::now();
			return std::chrono::duration<double>(now - m_prev).count();
		}

		/**
		* @brief クラス作成時からの経過時間を取得します。
		* @return double クラス作成時からの経過時間。
		*/
		double GetElapsedTime() const
		{
			TimePoint now = std::chrono::high_resolution_clock::now();
			return std::chrono::duration<double>(now - m_start).count();
		}

	private:
		//! クラス作成時の時間。
		TimePoint m_start;
		//! 前回取得した時間。
		TimePoint m_prev;
	};
}

#endif //!___SRC_COMMON_CORE_UTILS_TIMER_H___