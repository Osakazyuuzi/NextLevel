#ifndef ___SRC_COMMON_CORE_UTILS_TIMER_H___
#define ___SRC_COMMON_CORE_UTILS_TIMER_H___

#include <chrono>

namespace NextLevel
{
	/**
	* @class Timer
	* @brief �����x�Ŏ��Ԃ��v������N���X�B
	*/
	class Timer {
		//! �ʖ��G�C���A�X
		using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;
	
	public:
		/**
		* @brief �R���X�g���N�^�B
		*/
		Timer() 
			: m_start(std::chrono::high_resolution_clock::now())
			, m_prev(m_start)
		{}

		/**
		* @brief �O�񎞊Ԃ����Z�b�g���܂��B
		*/
		void Reset()
		{
			m_prev = std::chrono::high_resolution_clock::now();
		}

		/**
		* @brief �O�񃊃Z�b�g���Ԃ���̌o�ߎ��Ԃ��擾���܂��B
		* @return double �O�񃊃Z�b�g���Ԃ���̌o�ߎ��ԁB
		*/
		double GetDeltaTime()
		{
			TimePoint now = std::chrono::high_resolution_clock::now();
			return std::chrono::duration<double>(now - m_prev).count();
		}

		/**
		* @brief �N���X�쐬������̌o�ߎ��Ԃ��擾���܂��B
		* @return double �N���X�쐬������̌o�ߎ��ԁB
		*/
		double GetElapsedTime() const
		{
			TimePoint now = std::chrono::high_resolution_clock::now();
			return std::chrono::duration<double>(now - m_start).count();
		}

	private:
		//! �N���X�쐬���̎��ԁB
		TimePoint m_start;
		//! �O��擾�������ԁB
		TimePoint m_prev;
	};
}

#endif //!___SRC_COMMON_CORE_UTILS_TIMER_H___