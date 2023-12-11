#ifndef ___SRC_COMMON_CORE_ECS_SYSTEMBASE_H___
#define ___SRC_COMMON_CORE_ECS_SYSTEMBASE_H___

#include "Archetype.h"

namespace NextLevel
{
	namespace ecs
	{
		class World;
		class EntityManager;

		/**
		* @class SystemBase
		* @brief システムベースクラス。全てのシステムはこれを継承する。
		*/
		class SystemBase
		{
		public:
			/**
			* @brief コンストラクタ。
			* @param _pWorld 属するワールドへのポインタ。
			*/
			SystemBase(World* _pWorld)
				: m_pWorld(_pWorld)
			{}

			/**
			* @brief 最初に一度のみ処理を実行します。
			*/
			virtual void Init() = 0;

			/**
			* @brief 処理を実行します。
			* @param _deltaTime 前フレームとの差[ms]
			*/
			virtual void Update(float _deltaTime) {}

			/**
			* @brief 描画を実行します。
			*/
			virtual void Draw() {}

			/**
			* @brief 実行するのに必要とするアーキタイプを取得します。
			* @return 実行するのに必要なアーキタイプ。
			*/
			inline const Archetype& GetArchetype() const noexcept
			{
				return m_Archetype;
			}

			/**
			* @brief 属しているワールドのエンティティマネージャーを取得します。
			* @return std::shared_ptr<EntityManager> 属しているワールドのエンティティマネージャー。
			*/
			std::shared_ptr<EntityManager> GetEntityManager();

		protected:
			//! 実行するのに必要とするアーキタイプ。
			Archetype m_Archetype;

		private:
			//! 属しているワールドへのポインタ。
			World* m_pWorld = nullptr;
		};
	}
}
#endif //!___SRC_COMMON_CORE_ECS_SYSTEMBASE_H___