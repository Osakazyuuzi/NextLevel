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
			// �����J�������쐬
			Entity camera = m_pEntityManager->CreateEntity();
			m_pEntityManager->AddComponent<component::CDebugCamera>(camera);
			m_pEntityManager->SetComponent(camera, component::CDebugCamera());
			m_pEntityManager->ChangeName(camera, "DebugCamera");

			// �e�X�g���f�����쐬
			Entity model = m_pEntityManager->CreateEntity();
			m_pEntityManager->AddComponent<component::CMeshRenderer>(model);
			m_pEntityManager->SetComponent(model, component::CMeshRenderer());
			component::CTransform trans;
			trans.m_position.z() = 5.0f;
			m_pEntityManager->SetComponent(model, trans);
			m_pEntityManager->ChangeName(model, "TestModel");

			// �����V�X�e��������
			AddSystem<system::SOperationDebugCamera>(0);

			// �����_�����O�p�C�v���C�����\�z
			m_RenderSystemList.push_back(std::make_shared<system::SBindDebugCamera>(this));
			m_RenderSystemList.back()->Init();
			m_RenderSystemList.push_back(std::make_shared<system::SMeshRenderer>(this)); // �ʏ�`��
			m_RenderSystemList.back()->Init();
		}

		/**
		* @brief �X�V�������s���܂��B
		* @param _deltaTime �O�t���[������̌o�ߎ��ԁB
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
		* @brief �`�揈�����s���܂��B
		*/
		void World::Draw()
		{
			// �����_�����O���s
			for (auto system : m_RenderSystemList)
				system->Draw();
		}
	}
}