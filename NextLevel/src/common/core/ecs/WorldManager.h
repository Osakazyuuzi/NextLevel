#ifndef ___SRC_COMMON_CORE_ECS_WORLDMANAGER_H___
#define ___SRC_COMMON_CORE_ECS_WORLDMANAGER_H___

#include <list>
#include <memory>
#include <unordered_map>
#include <string>

#include "World.h"

namespace NextLevel
{
	namespace ecs
	{
		/**
		* @class WorldManager
		* @brief �S�Ẵ��[���h���Ǘ�����N���X�B
		*/
		class WorldManager {
		private:
			/**
			* @brief �R���X�g���N�^�B
			*/
			WorldManager() = default;

			/**
			* @brief �f�X�g���N�^�B
			*/
			~WorldManager() = default;

		public:
			/**
			* @brief �C���X�^���X���擾���܂��B
			* @return WorldManager& �C���X�^���X�B
			*/
			static WorldManager& GetInstance()
			{
				static WorldManager Instance;
				return Instance;
			}

		private:
			/**
			* @brief ���O�̔��␳�����܂��B
			* @param _name �␳���閼�O�B
			* @param _num �␳�񐔁B
			* @return std::string �␳�������O�B
			*/
			std::string NameSetting(std::string _name, int _num)
			{
				// ��薳���̏ꍇ�A���������Ԃ��B
				if (m_worldMap.find(_name) == m_worldMap.end())
					return _name;

				std::string newName;

				if (_num == 0)
				{
					newName = _name + " (1)";
				}
				else
				{
					// �����̐����𑝂₷
					newName = _name.substr(0, _name.find_last_of(" "));
					newName += " (" + std::to_string(_num + 1) + ")";
				}

				// �ēx������s��
				return NameSetting(newName, _num + 1);
			}

		public:
			/**
			* @brief ���[���h���쐬���܂��B
			* @param _name ���[���h���B
			*/
			std::shared_ptr<World> CreateWorld(std::string _name = "Default")
			{
				// ���O��␳���܂��B
				std::string name = _name;
				name = NameSetting(_name, 0);

				// �V�K�쐬
				m_worldMap[name] = std::make_shared<World>();
				m_worldMap[name]->SetName(name);

				return m_worldMap[name];
			}

			void Update(float _deltaTime)
			{
				if (m_currentWorldHierarchy.size())
					m_currentWorldHierarchy.back()->Update(_deltaTime);
			}

			void Draw() {

				if (m_currentWorldHierarchy.size())
					m_currentWorldHierarchy.back()->Draw();
			}

			std::list<std::shared_ptr<World>>& GetCurrentWorldHierarchy()
			{
				return m_currentWorldHierarchy;
			}

		private:
			//! ���[���h�}�b�v
			std::unordered_map<std::string, std::shared_ptr<World>> m_worldMap;
			//! ���݂̃��[���h�q�G�����L�[
			std::list<std::shared_ptr<World>> m_currentWorldHierarchy;
		};
	}
}

#endif //!___SRC_COMMON_CORE_ECS_WORLDMANAGER_H___