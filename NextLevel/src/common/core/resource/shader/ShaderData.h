#ifndef ___SRC_COMMON_CORE_RESOURCE_SHADER_SHADERDATA_H___
#define ___SRC_COMMON_CORE_RESOURCE_SHADER_SHADERDATA_H___

#include <d3d11.h>
#include <string>
#include <wrl.h>

namespace NextLevel
{
	namespace resource
	{
		/**
		* @class ShaderData
		* @brief シェーダーデータを管理するクラス。
		*/
		class ShaderData {
		public:
			/**
			* @brief シェーダーの種類を列挙。
			*/
			enum EnKind {
				enVertex,	// VertexShader
				enPixel,	// PixelShader
				enMax
			};

			/**
			* @brief シェーダーをロードします。
			* @param _kind シェーダーの種類。
			* @param _fileName シェーダーファイル名。
			* @return bool ロード成功したらtrue。
			*/
			bool Load(EnKind _kind, const char* _fileName);

			/**
			* @brief シェーダーをバインドします。
			*/
			void Bind();

			/**
			* @brief シェーダーの種類を取得します。
			* @return EnKind シェーダーの種類。
			*/
			const EnKind GetKind() const noexcept {
				return m_enKind;
			}

			/**
			* @brief シェーダーファイル名を取得します。
			* @return std::string シェーダーファイル名。
			*/
			const std::string GetFileName() const noexcept {
				return m_szFileName;
			}

		private:
			//! シェーダーの種類。
			EnKind m_enKind;
			//! 頂点シェーダーデータ。
			Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVS;
			//! インプットレイアウト。
			Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
			//! ピクセルシェーダーデータ。
			Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPS;
			//! シェーダーファイル名。
			std::string m_szFileName;
		};
	}
}

#endif //!___SRC_COMMON_CORE_RESOURCE_SHADER_SHADERDATA_H____