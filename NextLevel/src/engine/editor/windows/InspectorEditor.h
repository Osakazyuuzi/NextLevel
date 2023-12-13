#ifndef ___SRC_ENGINE_EDITOR_WINDOWS_INSPECTOREDITOR_H___
#define ___SRC_ENGINE_EDITOR_WINDOWS_INSPECTOREDITOR_H___

#include <string>

namespace NextLevel
{
	/**
	* @class InspectorEditor
	* @brief �C���X�y�N�^�[�̃G�f�B�^�[�N���X�B
	*/
	class InspectorEditor {
	public:
		/**
		* @brief �G�f�B�^��`�悵�܂��B
		*/
		void Draw();

	private:
		std::string m_szSelectName;
	private:
		// ���[���h
		char m_szSetWorldName[255];
		bool m_bWorldNameChanging;
		int m_nSelectSystemIndex = 0;
	private:
		// �G���e�B�e�B
		char m_szSetEntityName[255];
		bool m_bEntityNameChanging;
		int m_nSelectComponentIndex = 0;
	};
}

#endif //!___SRC_ENGINE_EDITOR_WINDOWS_INSPECTOREDITOR_H___