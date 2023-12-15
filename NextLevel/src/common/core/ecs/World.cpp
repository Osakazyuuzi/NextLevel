#include "World.h"

#include "EntityManager.h"
#include "SystemBase.h"

#include "system/SBindCamera.h"
#include "system/SMeshRenderer.h"

#include "component/CTransform.h"
#include "component/CCamera.h"

#include "EntityManager.h"

namespace NextLevel
{
	namespace ecs
	{
		World::World()
		{
			m_pEntityManager = std::make_shared<EntityManager>(this);
		}

		void World::Init()
		{
			// 初期カメラを作成
			Entity camera = m_pEntityManager->CreateEntity();
			m_pEntityManager->AddComponent<component::CCamera>(camera);

			// レンダリングパイプラインを構築
			m_RenderSystemList.push_back(std::make_shared<system::SBindCamera>(this));
			m_RenderSystemList.back()->Init();
			m_RenderSystemList.push_back(std::make_shared<system::SMeshRenderer>(this)); // 通常描画
			m_RenderSystemList.back()->Init();
		}

		/**
		* @brief 更新処理を行います。
		* @param _deltaTime 前フレームからの経過時間。
		*/
		void World::Update(float _deltaTime)
		{
			for (auto systems : m_SystemList)
			{
				for (auto&& system : systems)
				{
					system->Update(_deltaTime);
				}
			}
		}

		/**
		* @brief 描画処理を行います。
		*/
		void World::Draw()
		{
			// レンダリング実行
			for (auto system : m_RenderSystemList)
				system->Draw();
		}
	}
}