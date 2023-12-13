#ifndef ___SRC_COMMON_CORE_ECS_ENTITY_H___
#define ___SRC_COMMON_CORE_ECS_ENTITY_H___

#include "Utils.h"

namespace NextLevel
{
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
				: m_Identifier(NULL)
			{
				utils::SetIndex(m_Identifier, _index);
				utils::SetVersion(m_Identifier, _version);
			}

			/**
			* @brief コンストラクタ
			* @param _Identifier エンティティの識別ビット。
			*/
			Entity(const utils::EntityIdentifier _Identifier)
				: m_Identifier(_Identifier)
			{}

			//! エンティティの一意の識別ビット。
			utils::EntityIdentifier m_Identifier;
		};
	}
}
#endif //!___SRC_COMMON_CORE_ECS_ENTITY_H___