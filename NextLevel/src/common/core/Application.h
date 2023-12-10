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
	* @brief �A�v���P�[�V�����N���X�B
	*/
	class Application {
	public:
		/**
		* @brief ���������������܂��B
		*/
		void Init();

		/**
		* @brief �I�����������܂��B
		*/
		void Uninit();

		/**
		* @brief ���������s���܂��B
		*/
		void Run();

	private:
		/**
		* @brief ���t���[�����������s���܂��B
		* @param _deltaTime �O�t���[������̌o�ߎ��ԁB
		*/
		void DoFrame(float _deltaTime);

	private:
		//! �E�B���h�E�C���X�^���X
		std::shared_ptr<IWindow> m_window;
		//! �Ō�̎��s����
		unsigned long m_dwExecLastTime;
		//! ���ݎ���
		unsigned long m_dwCurrentTime;
		//! �t���[�����[�g
		float m_fFps = 60.0f;
		//! �^�C���X�P�[��
		float m_fTimeScale = 1.0f;
	};
}

#endif //!___SRC_CORE_ENGINE_H___