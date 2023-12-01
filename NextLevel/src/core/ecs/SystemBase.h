#ifndef ___SRC_CORE_ECS_SYSTEMBASE_H___
#define ___SRC_CORE_ECS_SYSTEMBASE_H___

#include "Archetype.h"

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
		* @brief 毎フレーム処理を実行します。
		* @param _deltaTime 前フレームとの差[ms]
		*/
		virtual void Update(float _deltaTime) = 0;

		/**
		* @brief 実行するのに必要とするアーキタイプを取得します。
		* @return 実行するのに必要なアーキタイプ。
		*/
		const Archetype& GetArchetype() const noexcept
		{
			return m_Archetype;
		}

		/**
		* @brief 属しているワールドのエンティティマネージャーを取得します。
		* @return EntityManager* 属しているワールドのエンティティマネージャー。
		*/
		EntityManager* GetEntityManager();

	protected:
		//! 実行するのに必要とするアーキタイプ。
		Archetype m_Archetype;

	private:
		//! 属しているワールドへのポインタ。
		World* m_pWorld = nullptr;
	};
}

#endif //!___SRC_CORE_ECS_SYSTEMBASE_H___