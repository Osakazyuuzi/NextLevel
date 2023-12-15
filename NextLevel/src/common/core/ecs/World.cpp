#include "World.h"

#include "EntityManager.h"
#include "SystemBase.h"

#include "system/SBindCamera.h"
#include "system/SBindDebugCamera.h"
#include "system/SMeshRenderer.h"
#include "system/SOperationDebugCamera.h"

#include "component/CTransform.h"
#include "component/CCamera.h"
#include "component/CDebugCamera.h"
#include "component/CMeshRenderer.h"

#include "EntityManager.h"

#include "src/common/core/resource/model/ModelManager.h"

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
			m_pEntityManager->AddComponent<component::CDebugCamera>(camera);
			m_pEntityManager->SetComponent(camera, component::CDebugCamera());
			m_pEntityManager->ChangeName(camera, "DebugCamera");

			// テストモデルを作成
			Entity model = m_pEntityManager->CreateEntity();
			m_pEntityManager->AddComponent<component::CMeshRenderer>(model);
			m_pEntityManager->SetComponent(model, component::CMeshRenderer());
			component::CTransform trans;
			trans.m_position.z() = 5.0f;
			m_pEntityManager->SetComponent(model, trans);
			m_pEntityManager->ChangeName(model, "TestModel");

			// 初期システムを高陸
			AddSystem<system::SOperationDebugCamera>(0);

			// レンダリングパイプラインを構築
			m_RenderSystemList.push_back(std::make_shared<system::SBindDebugCamera>(this));
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