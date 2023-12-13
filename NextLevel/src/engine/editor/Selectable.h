#ifndef ___SRC_ENGINE_EDITOR_SELECTABLE_H___
#define ___SRC_ENGINE_EDITOR_SELECTABLE_H___

#include <string>
#include "src/common/core/ecs/Utils.h"

namespace NextLevel
{
	/**
	* @class SelectableInfo
	* @brief �Z���N�g�����Ǘ�����N���X�B
	*/
	class SelectableInfo {
	private:
		/**
		* @brief �R���X�g���N�^�B
		*/
		SelectableInfo() = default;

		/**
		* @brief �f�X�g���N�^�B
		*/
		~SelectableInfo() = default;

	public:
		/**
		* @brief �C���X�^���X���擾���܂��B
		* @return SelectableInfo& �C���X�^���X�B
		*/
		static SelectableInfo& GetInstance()
		{
			static SelectableInfo instance;
			return instance;
		}

	public:
		//! �Z���N�g�ł�����
		enum EnSelect {
			enNone,
			enEntity,
			enTexture,
			enMaterial,
			enModel,
			enShader,
			enWorld
		};

		/**
		* @brief �I����ނ�ݒ肵�܂��B
		* @param _enSelect �I����ށB
		*/
		void SetEnSelect(EnSelect _enSelect)
		{
			m_enSelect = _enSelect;
		}

		/**
		* @brief �I����ނ��擾���܂��B
		* @return EnSelect �I����ށB
		*/
		EnSelect GetEnSelect() const noexcept
		{
			return m_enSelect;
		}

		/**
		* @brief �ȈՎ��ʖ���ݒ肵�܂��B
		* @param _szName ���ʖ��B
		*/
		void SetName(std::string _szName)
		{
			m_szName = _szName;
		}

		/**
		* @brief �ȈՎ��ʖ����擾���܂��B
		* @return std::string ���ʖ��B
		*/
		std::string GetName()
		{
			return m_szName;
		}

		/**
		* @brief �ȈՎ���ID��ݒ肵�܂��B
		* @param _nId ����ID�B
		*/
		void SetID(int _nId)
		{
			m_nId = _nId;
		}

		/**
		* @brief �ȈՎ���ID���擾���܂��B
		* @return int ����ID�B
		*/
		int GetID() const noexcept
		{
			return m_nId;
		}

		/**
		* @brief �ȈՎ��ʃr�b�g��ݒ肵�܂��B
		* @param _bit ���ʃr�b�g�B
		*/
		void SetBit(ecs::utils::EntityIdentifier _bit)
		{
			m_bit = _bit;
		}

		/**
		* @brief �ȈՎ��ʃr�b�g���擾���܂��B
		* @return ecs::utils::EntityIdentifier ���ʃr�b�g�B
		*/
		ecs::utils::EntityIdentifier GetBit() const noexcept
		{
			return m_bit;
		}

	private:
		//! �I����ޏ��
		EnSelect m_enSelect = enNone;
		//! �I���ȈՎ��ʏ��
		std::string m_szName;
		int m_nId;
		ecs::utils::EntityIdentifier m_bit;
	};
}

#endif //!___SRC_ENGINE_EDITOR_SELECTABLE_H___