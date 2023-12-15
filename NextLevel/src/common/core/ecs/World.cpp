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
			// �����J�������쐬
			Entity camera = m_pEntityManager->CreateEntity();
			m_pEntityManager->AddComponent<component::CCamera>(camera);

			// �����_�����O�p�C�v���C�����\�z
			m_RenderSystemList.push_back(std::make_shared<system::SBindCamera>(this));
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