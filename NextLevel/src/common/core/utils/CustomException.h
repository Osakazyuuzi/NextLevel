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
	* @brief 例外を投げるクラス。
	*/
	class CustomException : public std::exception
	{
	public:
		/**
		* @brief コンストラクタ。
		* @param _szFunction 関数名。
		* @param _szFile ファイル名。
		* @param _nLine 行数。
		* @param _szCustomMessage カスタムメッセージ。
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
		* @brief 例外内容を取得します。
		* @return const char* 例外内容。
		*/
		const char* what() const noexcept override
		{
			return m_szWhatBuf.c_str();
		}

	private:
		//! 例外が起きた行数
		int m_nLine;
		//! 例外が起きた関数名
		std::string m_szFunction;
		//! 例外が起きたファイル名
		std::string m_szFile;
		//! カスタム例外メッセージ
		std::string m_szCustomMessage;
		//! 例外メッセージ
		std::string m_szWhatBuf;
	};
}

#endif //!___SRC_COMMON_CORE_UTILS_COSTOMEXCEPTION_H___