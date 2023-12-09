#ifndef ___SRC_COMMON_CORE_UTILS_COSTOMEXCEPTION_H___
#define ___SRC_COMMON_CORE_UTILS_COSTOMEXCEPTION_H___

#include <exception>
#include <string>
#include <iostream>
#include <sstream>

namespace NextLevel
{
	/**
	* @class CustomException
	* @brief ��O�𓊂���N���X�B
	*/
	class CustomException : public std::exception
	{
	public:
		/**
		* @brief �R���X�g���N�^�B
		* @param _szFunction �֐����B
		* @param _szFile �t�@�C�����B
		* @param _nLine �s���B
		* @param _szCustomMessage �J�X�^�����b�Z�[�W�B
		*/
		CustomException(
			const char* _szFunction,
			const char* _szFile,
			int _nLine,
			const char* _szCustomMessage = "")
			: m_szFunction(_szFunction)
			, m_szFile(_szFile)
			, m_nLine(_nLine)
			, m_szCustomMessage(_szCustomMessage)
		{
			std::stringstream oss;

			oss << "Exception thorwn in " << m_szFunction << std::endl
				<< "File: " << m_szFile << std::endl
				<< "Line: " << m_nLine << std::endl
				<< "Message: " << m_szCustomMessage;

			m_szWhatBuf = oss.str();
		}

		/**
		* @brief ��O���e���擾���܂��B
		* @return const char* ��O���e�B
		*/
		const char* what() const noexcept override
		{
			return m_szWhatBuf.c_str();
		}

	private:
		//! ��O���N�����s��
		int m_nLine;
		//! ��O���N�����֐���
		std::string m_szFunction;
		//! ��O���N�����t�@�C����
		std::string m_szFile;
		//! �J�X�^����O���b�Z�[�W
		std::string m_szCustomMessage;
		//! ��O���b�Z�[�W
		std::string m_szWhatBuf;
	};
}

#endif //!___SRC_COMMON_CORE_UTILS_COSTOMEXCEPTION_H___