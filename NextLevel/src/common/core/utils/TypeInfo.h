#ifndef ___SRC_COMMON_CORE_UTILS_TYPEINFO_H___
#define ___SRC_COMMON_CORE_UTILS_TYPEINFO_H___

#include <typeinfo>
#include <string>
#include <mutex>

namespace NextLevel
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
		* @brief 特定の型の情報を保持するクラス。
		*/
		template <typename T>
		class TypeInfo
		{
		private:
			//! 型ID
			static std::size_t m_Id;
			//! 型名
			static std::string m_szName;

		private:
			/**
			* @brief 型情報を登録する。
			*/
			static inline void Register()
			{
				std::lock_guard<std::mutex> lock(m_Mutex);
				TypeInfo<T>::m_Id = TypeManager::m_NextId++;
				TypeInfo<T>::m_szName = typeid(T).name();
			}

		public:

			/**
			* @brief 型IDを取得する。
			* @return const std::size_t& 型ID
			*/
			static inline const std::size_t& GetID()
			{
				if (TypeInfo<T>::m_Id == 0) Register();

				return m_Id;
			}

			/**
			* @brief 型名を取得する。
			* @return const std::string& 型名
			*/
			static inline const std::string& GetName()
			{
				if (TypeInfo<T>::m_Id == 0) Register();
				return m_szName;
			}
		};

	private:
		//! 型情報登録時の排他制御用ミューテックス
		static std::mutex m_Mutex;
		//! 次に割り当てる型ID
		static std::size_t m_NextId;
	};

	template <typename T>
	std::size_t TypeManager::TypeInfo<T>::m_Id = 0;

	template <typename T>
	std::string TypeManager::TypeInfo<T>::m_szName;
}

#endif //!___SRC_COMMON_CORE_UTILS_TYPEINFO_H___