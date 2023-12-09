#ifndef ___SRC_COMMON_CORE_UTILS_TYPEINFO_H___
#define ___SRC_COMMON_CORE_UTILS_TYPEINFO_H___

#include <typeinfo>
#include <string>
#include <mutex>

namespace NextLevel
{
	/**
	* @class TypeManager
	* @brief �^�����Ǘ�����N���X
	*/
	class TypeManager
	{
	public:
		/**
		* @class TypeInfo
		* @brief ����̌^�̏���ێ�����N���X�B
		*/
		template <typename T>
		class TypeInfo
		{
		private:
			//! �^ID
			static std::size_t m_Id;
			//! �^��
			static std::string m_szName;

		private:
			/**
			* @brief �^����o�^����B
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
			* @brief �^ID���擾����B
			* @return const std::size_t& �^ID
			*/
			static inline const std::size_t& GetID()
			{
				std::lock_guard<std::mutex> lock(m_Mutex);
				Register();
				return m_Id;
			}

			/**
			* @brief �^�����擾����B
			* @return const std::string& �^��
			*/
			static inline const std::string& GetName()
			{
				std::lock_guard<std::mutex> lock(m_Mutex);
				Register();
				return m_szName;
			}
		};

	private:
		//! �^���o�^���̔r������p�~���[�e�b�N�X
		static std::mutex m_Mutex;
		//! ���Ɋ��蓖�Ă�^ID
		static std::size_t m_NextId;
	};

	template <typename T>
	std::size_t TypeManager::TypeInfo<T>::m_Id = 0;

	template <typename T>
	std::string TypeManager::TypeInfo<T>::m_szName;
}

#endif //!___SRC_COMMON_CORE_UTILS_TYPEINFO_H___