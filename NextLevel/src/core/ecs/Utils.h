#ifndef ___SRC_CORE_ECS_UTILS_H___
#define ___SRC_CORE_ECS_UTILS_H___

namespace ecs
{
	namespace utils
	{
		//! コンポーネントの種類の最大数。
		constexpr std::size_t cMaxComponentSize = 128;

        //! エンティティの管理インデックスの型。
        using EntityIndex = std::uint32_t;
        //! エンティティの管理バージョンの型。
        using EntityVersion = std::uint32_t;
        //! エンティティの識別IDの型。
        using EntityIdentifier = std::uint64_t;

        /**
        * @brief 識別IDにインデックスを設定する。
        * @param _identifier インデックスを適用するエンティティの識別ID。
        * @param _index 設定するインデックス値。
        */
        static void SetIndex(EntityIdentifier& _identifier, EntityIndex _index)
        {
            constexpr EntityIdentifier versionMask = static_cast<EntityIdentifier>(EntityVersion(-1));
            _identifier = (_identifier & versionMask) | (static_cast<EntityIdentifier>(_index) << 32);
        }

        /**
        * @brief 識別IDにバージョンを設定する。
        * @param _identifier& バージョンを適用するエンティティの識別ID。
        * @param _version 設定するバージョン値。
        */
        static void SetVersion(EntityIdentifier& _identifier, EntityVersion _version)
        {
            constexpr EntityIdentifier indexMask = ~static_cast<EntityIdentifier>(EntityVersion(-1));
            _identifier = (_identifier & indexMask) | _version;
        }

        /**
        * @brief 識別IDからインデックスを取得する。
        * @param _identifier エンティティの識別ID。
        * @return エンティティのインデックス。
        */
        static EntityIndex GetIndex(const EntityIdentifier& _identifier)
        {
            return static_cast<EntityIndex>(_identifier >> 32);
        }

        /**
        * @brief 識別IDからバージョンを取得する。
        * @param _identifier エンティティの識別ID。
        * @return エンティティのバージョン。
        */
        static EntityVersion GetVersion(const EntityIdentifier& _identifier)
        {
            return static_cast<EntityVersion>(_identifier);
        }
	}
}

#endif //!___SRC_CORE_ECS_UTILS_H___