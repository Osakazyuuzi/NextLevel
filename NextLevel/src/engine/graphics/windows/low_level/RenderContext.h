#ifndef ___SRC_CORE_GRAPHICS_LOWLEVEL_RENDERCONTEXT_H___
#define ___SRC_CORE_GRAPHICS_LOWLEVEL_RENDERCONTEXT_H___

#include <d3d11.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <memory>
#include "src/common/core/resource/texture/TextureData.h"
#include "src/common/core/resource/shader/ShaderData.h"
#include "src/common/core/resource/material/MaterialData.h"

#pragma comment(lib, "d3d11.lib")

namespace NextLevel
{
	namespace graphics
	{
		namespace lowlevel
		{
			class RenderTarget;
			class DepthStencil;

			class RenderContext
			{
			public:
				void Init();
				void Uninit();

				void SetVertexBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer> _vtxBuf, UINT _stride);
				void SetIndexBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer> _idxBuf);
				void SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY _topology);
				void SetViewport(float _width, float _height);
				D3D11_VIEWPORT GetViewport() const noexcept;

				void SetRenderTargets(UINT num, std::shared_ptr<RenderTarget> _rtv[], std::shared_ptr<DepthStencil> _dsv);

				void ClearRenderTargetView(std::shared_ptr<RenderTarget> _rtv, DirectX::XMFLOAT4 _color);

				void ClearDepthStencilView(std::shared_ptr<DepthStencil> _dsv);

				void DrawIndexed(UINT _indexCount);

				void SetWorldMatrix(DirectX::XMFLOAT4X4* _worldMat);

				void SetViewMatrix(DirectX::XMFLOAT4X4* _viewMat);

				void SetProjectionMatrix(DirectX::XMFLOAT4X4* _projectionMat);

				void SetMaterial(resource::MaterialData _material);

				void SetTexture(UINT _slot, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _pTex, 
					resource::ShaderData::EnKind _enKind = resource::ShaderData::EnKind::enPixel);

				enum EnCullingMode {
					enCULLING_NONE,
					enCULLING_FRONT,
					enCULLING_BACK,
					enCULLING_MAX
				};

				void SetCullingMode(EnCullingMode _enCullMode);

				enum EnDepthMode {
					enDEPTH_OFF,
					enDEPTH_ON,
					enDEPTH_MAX
				};

				void SetDepthMode(EnDepthMode _enDepthMode);

				enum EnBlendMode {
					enBLEND_NONE,
					enBLEND_ALPHA,
					enBLEND_ADD,
					enBLEND_ADDALPHA,
					enBLEND_SUB,
					enBLEND_SCREEN,
					enBLEND_MAX
				};

				void SetBlendMode(EnBlendMode _enBlendMode);

				enum EnSamplerStateMode
				{
					enSAMPLER_LINEAR,
					enSAMPLER_POINT,
					enSAMPLER_MAX
				};

				void SetSamplerStateMode(EnSamplerStateMode _enSamplerStateMode);

			private:
				enum CONSTANT_BUFFER { MAX_CONSTANT_BUFFER = 8 };
				enum SHADER_RESOURCE { MAX_SHADER_RESOURCE = 4 };

				Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_pDepthState[EnDepthMode::enDEPTH_MAX];
				Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_pRasterizerState[EnCullingMode::enCULLING_MAX];
				Microsoft::WRL::ComPtr<ID3D11BlendState> m_pBlendState[EnBlendMode::enBLEND_MAX];
				Microsoft::WRL::ComPtr<ID3D11SamplerState> m_pSamplerState[EnSamplerStateMode::enSAMPLER_MAX];
				D3D11_VIEWPORT m_currentViewport;
				Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffers[MAX_CONSTANT_BUFFER] = { nullptr };
				Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pShaderResources[MAX_SHADER_RESOURCE] = { nullptr };
			};
		}
	}
}

#endif //!___SRC_CORE_GRAPHICS_LOWLEVEL_RENDERCONTEXT_H___