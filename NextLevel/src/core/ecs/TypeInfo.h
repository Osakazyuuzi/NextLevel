#ifndef ___SRC_CORE_ECS_TYPEINFO_H___
#define ___SRC_CORE_ECS_TYPEINFO_H___

#include <typeinfo>
#include <string>
#include <mutex>

namespace ecs
{
	/**
	* @class TypeManager
	* @brief 型情報を管理するクラス
	*/
	class TypeManager
	{
	public:
		/**
		* @class TypeInfo
		* @brief 特定のコンポーネント型の情報を保持するクラス。
		*/
		template <typename CompT>
		class TypeInfo
		{
		private:
			//! コンポーネントの型ID
			static std::size_t m_Id;
			//! コンポーネントの型名
			static std::string m_szName;

		private:
			/**
			* @brief コンポーネントの型情報を登録する。
			*/
			static inline void Register()
			{
				if (TypeInfo<CompT>::m_Id == 0)
				{
					TypeInfo<CompT>::m_Id = TypeManager::m_NextId++;
					TypeInfo<CompT>::m_szName = typeid(CompT).name();
				}
			}

		public:

			/**
			* @brief コンポーネントの型IDを取得する。
			* @return const std::size_t& 型ID
			*/
			static inline const std::size_t& GetID()
			{
				std::lock_guard<std::mutex> lock(m_Mutex);
				Register();
				return m_Id;
			}

			/**
			* @brief コンポーネントの型名を取得する。
			* @return const std::string& 型名
			*/
			static inline const std::string& GetName()
			{
				std::lock_guard<std::mutex> lock(m_Mutex);
				Register();
				return m_szName;
			}
		};

	private:
		//! 型情報登録時の排他制御用ミューテックス
		static std::mutex m_Mutex;
		//! 次に割り当てる型ID
		static std::size_t m_NextId;
	};

	template <typename CompT>
	std::size_t TypeManager::TypeInfo<CompT>::m_Id = 0;

	template <typename CompT>
	std::string TypeManager::TypeInfo<CompT>::m_szName;
}

#endif //!___SRC_CORE_ECS_TYPEINFO_H___