#ifndef ___SRC_ENGINE_EDITOR_WINDOWS_INSPECTOREDITOR_H___
#define ___SRC_ENGINE_EDITOR_WINDOWS_INSPECTOREDITOR_H___

#include <string>

namespace NextLevel
{
	/**
	* @class InspectorEditor
	* @brief インスペクターのエディタークラス。
	*/
	class InspectorEditor {
	public:
		/**
		* @brief エディタを描画します。
		*/
		void Draw();

	private:
		std::string m_szSelectName;
	private:
		// ワールド
		char m_szSetWorldName[255];
		bool m_bWorldNameChanging;
		int m_nSelectSystemIndex = 0;
	private:
		// エンティティ
		char m_szSetEntityName[255];
		bool m_bEntityNameChanging;
		int m_nSelectComponentIndex = 0;
	};
}

#endif //!___SRC_ENGINE_EDITOR_WINDOWS_INSPECTOREDITOR_H___