#ifndef ___SRC_ENGINE_EDITOR_SELECTABLE_H___
#define ___SRC_ENGINE_EDITOR_SELECTABLE_H___

#include <string>
#include "src/common/core/ecs/Utils.h"

namespace NextLevel
{
	/**
	* @class SelectableInfo
	* @brief セレクト情報を管理するクラス。
	*/
	class SelectableInfo {
	private:
		/**
		* @brief コンストラクタ。
		*/
		SelectableInfo() = default;

		/**
		* @brief デストラクタ。
		*/
		~SelectableInfo() = default;

	public:
		/**
		* @brief インスタンスを取得します。
		* @return SelectableInfo& インスタンス。
		*/
		static SelectableInfo& GetInstance()
		{
			static SelectableInfo instance;
			return instance;
		}

	public:
		//! セレクトできる種類
		enum EnSelect {
			enNone,
			enEntity,
			enTexture,
			enMaterial,
			enModel,
			enShader,
			enWorld
		};

		/**
		* @brief 選択種類を設定します。
		* @param _enSelect 選択種類。
		*/
		void SetEnSelect(EnSelect _enSelect)
		{
			m_enSelect = _enSelect;
		}

		/**
		* @brief 選択種類を取得します。
		* @return EnSelect 選択種類。
		*/
		EnSelect GetEnSelect() const noexcept
		{
			return m_enSelect;
		}

		/**
		* @brief 簡易識別名を設定します。
		* @param _szName 識別名。
		*/
		void SetName(std::string _szName)
		{
			m_szName = _szName;
		}

		/**
		* @brief 簡易識別名を取得します。
		* @return std::string 識別名。
		*/
		std::string GetName()
		{
			return m_szName;
		}

		/**
		* @brief 簡易識別IDを設定します。
		* @param _nId 識別ID。
		*/
		void SetID(int _nId)
		{
			m_nId = _nId;
		}

		/**
		* @brief 簡易識別IDを取得します。
		* @return int 識別ID。
		*/
		int GetID() const noexcept
		{
			return m_nId;
		}

		/**
		* @brief 簡易識別ビットを設定します。
		* @param _bit 識別ビット。
		*/
		void SetBit(ecs::utils::EntityIdentifier _bit)
		{
			m_bit = _bit;
		}

		/**
		* @brief 簡易識別ビットを取得します。
		* @return ecs::utils::EntityIdentifier 識別ビット。
		*/
		ecs::utils::EntityIdentifier GetBit() const noexcept
		{
			return m_bit;
		}

	private:
		//! 選択種類情報
		EnSelect m_enSelect = enNone;
		//! 選択簡易識別情報
		std::string m_szName;
		int m_nId;
		ecs::utils::EntityIdentifier m_bit;
	};
}

#endif //!___SRC_ENGINE_EDITOR_SELECTABLE_H___