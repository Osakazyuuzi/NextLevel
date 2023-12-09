#ifndef ___SRC_COMMON_CORE_UTILS_TYPEINFO_H___
#define ___SRC_COMMON_CORE_UTILS_TYPEINFO_H___

#include <typeinfo>
#include <string>
#include <mutex>

namespace NextLevel
{
	/**
	* @class TypeManager
	* @brief Œ^î•ñ‚ğŠÇ—‚·‚éƒNƒ‰ƒX
	*/
	class TypeManager
	{
	public:
		/**
		* @class TypeInfo
		* @brief “Á’è‚ÌŒ^‚Ìî•ñ‚ğ•Û‚·‚éƒNƒ‰ƒXB
		*/
		template <typename T>
		class TypeInfo
		{
		private:
			//! Œ^ID
			static std::size_t m_Id;
			//! Œ^–¼
			static std::string m_szName;

		private:
			/**
			* @brief Œ^î•ñ‚ğ“o˜^‚·‚éB
			*/
			static inline void Register()
			{
				if (TypeInfo<T>::m_Id == 0)
				{
					TypeInfo<T>::m_Id = TypeManager::m_NextId++;
					TypeInfo<T>::m_szName = typeid(T).name();
				}
			}

		public:

			/**
			* @brief Œ^ID‚ğæ“¾‚·‚éB
			* @return const std::size_t& Œ^ID
			*/
			static inline const std::size_t& GetID()
			{
				std::lock_guard<std::mutex> lock(m_Mutex);
				Register();
				return m_Id;
			}

			/**
			* @brief Œ^–¼‚ğæ“¾‚·‚éB
			* @return const std::string& Œ^–¼
			*/
			static inline const std::string& GetName()
			{
				std::lock_guard<std::mutex> lock(m_Mutex);
				Register();
				return m_szName;
			}
		};

	private:
		//! Œ^î•ñ“o˜^‚Ì”r‘¼§Œä—pƒ~ƒ…[ƒeƒbƒNƒX
		static std::mutex m_Mutex;
		//! Ÿ‚ÉŠ„‚è“–‚Ä‚éŒ^ID
		static std::size_t m_NextId;
	};

	template <typename T>
	std::size_t TypeManager::TypeInfo<T>::m_Id = 0;

	template <typename T>
	std::string TypeManager::TypeInfo<T>::m_szName;
}

#endif //!___SRC_COMMON_CORE_UTILS_TYPEINFO_H___