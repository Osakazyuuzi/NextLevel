#ifndef ___SRC_CORE_ECS_ENTITY_H___
#define ___SRC_CORE_ECS_ENTITY_H___

#include "Utils.h"

namespace ecs
{
	/**
	* @struct Entity
	* @brief ECSアーキテクチャにおけるエンティティを表す構造体。
	*/
	struct Entity
	{
		/**
		* @brief コンストラクタ
		* @param _index エンティティのインデックス。
		* @param _version エンティティのバージョン
		*/
		Entity(const std::uint32_t _index, const std::uint32_t _version)
			: m_Idntifier(NULL)
		{
			utils::SetIndex(m_Idntifier, _index);
			utils::SetVersion(m_Idntifier, _version);
		}

		//! エンティティの一意の識別ビット。
		utils::EntityIdentifier m_Idntifier;
	};
}

#endif //!___SRC_CORE_ECS_ENTITY_H___