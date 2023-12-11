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
		* @brief 全てのワールドを管理するクラス。
		*/
		class WorldManager {
		private:
			/**
			* @brief コンストラクタ。
			*/
			WorldManager() = default;

			/**
			* @brief デストラクタ。
			*/
			~WorldManager() = default;

		public:
			/**
			* @brief インスタンスを取得します。
			* @return WorldManager& インスタンス。
			*/
			static WorldManager& GetInstance()
			{
				static WorldManager Instance;
				return Instance;
			}

		private:
			/**
			* @brief 名前の被り補正をします。
			* @param _name 補正する名前。
			* @param _num 補正回数。
			* @return std::string 補正した名前。
			*/
			std::string NameSetting(std::string _name, int _num)
			{
				// 被り無しの場合、成功判定を返す。
				if (m_worldMap.find(_name) == m_worldMap.end())
					return _name;

				std::string newName;

				if (_num == 0)
				{
					newName = _name + " (1)";
				}
				else
				{
					// 末尾の数字を増やす
					newName = _name.substr(0, _name.find_last_of(" "));
					newName += " (" + std::to_string(_num + 1) + ")";
				}

				// 再度判定を行う
				return NameSetting(newName, _num + 1);
			}

		public:
			/**
			* @brief ワールドを作成します。
			* @param _name ワールド名。
			*/
			std::shared_ptr<World> CreateWorld(std::string _name = "Default")
			{
				// 名前を補正します。
				std::string name = _name;
				name = NameSetting(_name, 0);

				// 新規作成
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
			//! ワールドマップ
			std::unordered_map<std::string, std::shared_ptr<World>> m_worldMap;
			//! 現在のワールドヒエラルキー
			std::list<std::shared_ptr<World>> m_currentWorldHierarchy;
		};
	}
}

#endif //!___SRC_COMMON_CORE_ECS_WORLDMANAGER_H___