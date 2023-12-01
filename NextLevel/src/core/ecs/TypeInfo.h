#ifndef ___SRC_CORE_ECS_TYPEINFO_H___
#define ___SRC_CORE_ECS_TYPEINFO_H___

#include <typeinfo>
#include <string>
#include <mutex>

namespace ecs
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
		* @brief ����̃R���|�[�l���g�^�̏���ێ�����N���X�B
		*/
		template <typename CompT>
		class TypeInfo
		{
		private:
			//! �R���|�[�l���g�̌^ID
			static std::size_t m_Id;
			//! �R���|�[�l���g�̌^��
			static std::string m_szName;

		private:
			/**
			* @brief �R���|�[�l���g�̌^����o�^����B
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
			* @brief �R���|�[�l���g�̌^ID���擾����B
			* @return const std::size_t& �^ID
			*/
			static inline const std::size_t& GetID()
			{
				std::lock_guard<std::mutex> lock(m_Mutex);
				Register();
				return m_Id;
			}

			/**
			* @brief �R���|�[�l���g�̌^�����擾����B
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

	template <typename CompT>
	std::size_t TypeManager::TypeInfo<CompT>::m_Id = 0;

	template <typename CompT>
	std::string TypeManager::TypeInfo<CompT>::m_szName;
}

#endif //!___SRC_CORE_ECS_TYPEINFO_H___