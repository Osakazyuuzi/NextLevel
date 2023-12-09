#include "RenderContext.h"

#include "../Graphics.h"
#include "RenderTarget.h"
#include "DepthStencil.h"

namespace NextLevel
{
	namespace graphics
	{
		namespace lowlevel
		{
			void RenderContext::Init()
			{
				HRESULT hr;

				//=== �J�����O�ݒ�
				D3D11_RASTERIZER_DESC rasterizer = {};
				D3D11_CULL_MODE cull[] = {
					D3D11_CULL_NONE,
					D3D11_CULL_FRONT,
					D3D11_CULL_BACK
				};
				rasterizer.FillMode = D3D11_FILL_SOLID;
				rasterizer.FrontCounterClockwise = true;
				for (int i = 0; i < 3; ++i)
				{
					rasterizer.CullMode = cull[i];
					hr = graphics::Graphics::GetInstance().GetDevice()->CreateRasterizerState(&rasterizer, &m_pRasterizerState[i]);
					if (FAILED(hr)) {
						// �G���[�o��
						MessageBox(nullptr, "Error:InitCullingMode", "Error", MB_ICONERROR | MB_OK);
						// �����𒆒f
						return;
					}
				}
				SetCullingMode(enCULLING_NONE);

				//=== �[�x�X�e�[�g
				D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
				depthStencilDesc.DepthEnable = TRUE;
				depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
				depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
				depthStencilDesc.StencilEnable = FALSE;
				graphics::Graphics::GetInstance().GetDevice()->CreateDepthStencilState(&depthStencilDesc, &m_pDepthState[enDEPTH_ON]);//�[�x�L���X�e�[�g
				depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
				graphics::Graphics::GetInstance().GetDevice()->CreateDepthStencilState(&depthStencilDesc, &m_pDepthState[enDEPTH_OFF]);//�[�x�����X�e�[�g
				SetDepthMode(enDEPTH_ON);

				//=== �A���t�@�u�����f�B���O
				// https://pgming-ctrl.com/directx11/blend/
				D3D11_BLEND_DESC blendDesc = {};
				blendDesc.AlphaToCoverageEnable = FALSE;
				blendDesc.IndependentBlendEnable = FALSE;
				blendDesc.RenderTarget[0].BlendEnable = TRUE;
				blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
				blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
				blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
				blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
				blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
				D3D11_BLEND blend[enBLEND_MAX][2] = {
					{D3D11_BLEND_ONE, D3D11_BLEND_ZERO},
					{D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA},
					{D3D11_BLEND_ONE, D3D11_BLEND_ONE},
					{D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_ONE},
					{D3D11_BLEND_ZERO, D3D11_BLEND_INV_SRC_COLOR},
					{D3D11_BLEND_INV_DEST_COLOR, D3D11_BLEND_ONE},
				};
				for (int i = 0; i < enBLEND_MAX; ++i)
				{
					blendDesc.RenderTarget[0].SrcBlend = blend[i][0];
					blendDesc.RenderTarget[0].DestBlend = blend[i][1];
					hr = graphics::Graphics::GetInstance().GetDevice()->CreateBlendState(&blendDesc, &m_pBlendState[i]);
					if (FAILED(hr)) {
						// �G���[�o��
						MessageBox(nullptr, "Error:InitBlendMode", "Error", MB_ICONERROR | MB_OK);
						// �����𒆒f
						return;
					}
				}
				SetBlendMode(enBLEND_ALPHA);

				//=== �T���v���[
				D3D11_SAMPLER_DESC samplerDesc = {};
				D3D11_FILTER filter[] = {
					D3D11_FILTER_MIN_MAG_MIP_LINEAR,
					D3D11_FILTER_MIN_MAG_MIP_POINT,
				};
				samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
				samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
				samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
				for (int i = 0; i < enSAMPLER_MAX; ++i)
				{
					samplerDesc.Filter = filter[i];
					hr = graphics::Graphics::GetInstance().GetDevice()->CreateSamplerState(&samplerDesc, &m_pSamplerState[i]);
					if (FAILED(hr)) {
						// �G���[�o��
						MessageBox(nullptr, "Error:InitSamplerStateMode", "Error", MB_ICONERROR | MB_OK);
						// �����𒆒f
						return;
					}
				}
				SetSamplerStateMode(enSAMPLER_LINEAR);

				//=== �萔�o�b�t�@�쐬
				D3D11_BUFFER_DESC bufferDesc{};
				bufferDesc.ByteWidth = sizeof(DirectX::XMFLOAT4X4);
				bufferDesc.Usage = D3D11_USAGE_DEFAULT;
				bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
				bufferDesc.CPUAccessFlags = 0;
				bufferDesc.MiscFlags = 0;
				bufferDesc.StructureByteStride = sizeof(float);

				// ���[���h�}�g���N�X
				graphics::Graphics::GetInstance().GetDevice()->CreateBuffer(&bufferDesc, NULL, &m_pConstantBuffers[0]);
				graphics::Graphics::GetInstance().GetDeviceContext()->VSSetConstantBuffers(0, 1, &m_pConstantBuffers[0]);

				// �r���[�}�g���N�X
				graphics::Graphics::GetInstance().GetDevice()->CreateBuffer(&bufferDesc, NULL, &m_pConstantBuffers[1]);
				graphics::Graphics::GetInstance().GetDeviceContext()->VSSetConstantBuffers(1, 1, &m_pConstantBuffers[1]);

				// �v���W�F�N�V�����}�g���N�X
				graphics::Graphics::GetInstance().GetDevice()->CreateBuffer(&bufferDesc, NULL, &m_pConstantBuffers[2]);
				graphics::Graphics::GetInstance().GetDeviceContext()->VSSetConstantBuffers(2, 1, &m_pConstantBuffers[2]);

				//// �T�C�Y��ݒ�
				//bufferDesc.ByteWidth = sizeof(Material);

				//// �}�e���A��
				//g_graphics->GetDevice()->CreateBuffer(&bufferDesc, NULL, &m_constantBuffers[3]);
				//g_graphics->GetContext()->VSSetConstantBuffers(3, 1, &m_constantBuffers[3]);
				//g_graphics->GetContext()->PSSetConstantBuffers(3, 1, &m_constantBuffers[3]);
			}

			/*
			[�֐��T�v]
			�I������
			*/
			void RenderContext::Uninit()
			{
				for (int i = 0; i < EnSamplerStateMode::enSAMPLER_MAX; i++)
					m_pSamplerState[i].Reset();
				for (int i = 0; i < EnBlendMode::enBLEND_MAX; i++)
					m_pBlendState[i].Reset();
				for (int i = 0; i < EnDepthMode::enDEPTH_MAX; i++)
					m_pDepthState[i].Reset();
				for (int i = 0; i < EnCullingMode::enCULLING_MAX; i++)
					m_pRasterizerState[i].Reset();
			}

			///*
			//[�֐��T�v]
			//���_�o�b�t�@��ݒ肷��

			//[����]
			//ID3D11Buffer*	vtxBuf	���_�o�b�t�@
			//*/
			//void RenderContext::SetVertexBuffer(ID3D11Buffer* vtxBuf, UINT stride)
			//{
			//	UINT offset = 0;
			//	g_graphics->GetContext()->IASetVertexBuffers(0, 1, &vtxBuf, &stride, &offset);
			//}

			///*
			//[�֐��T�v]
			//�C���f�b�N�X�o�b�t�@��ݒ肷��

			//[����]
			//ID3D11Buffer*	idxBuf	�C���f�b�N�X�o�b�t�@
			//*/
			//void RenderContext::SetIndexBuffer(ID3D11Buffer* idxBuf)
			//{
			//	g_graphics->GetContext()->IASetIndexBuffer(idxBuf, DXGI_FORMAT_R32_UINT, 0);
			//}


			/*
			[�֐��T�v]
			�v���~�e�B�u�̃g�|���W�[��ݒ肷��

			[����]
			D3D11_PRIMITIVE_TOPOLOGY	topology	�g�|���W�[�^�C�v
			*/
			void RenderContext::SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology)
			{
				graphics::Graphics::GetInstance().GetDeviceContext()->IASetPrimitiveTopology(topology);
			}

			/*
			[�֐��T�v]
			�r���[�|�[�g��ݒ肷��

			[����]
			float width		����
			float height	�c��
			*/
			void RenderContext::SetViewport(float width, float height)
			{
				m_currentViewport.TopLeftX = 0.0f;
				m_currentViewport.TopLeftY = 0.0f;
				m_currentViewport.Width = width;
				m_currentViewport.Height = height;
				m_currentViewport.MinDepth = 0.0f;
				m_currentViewport.MaxDepth = 1.0f;
				graphics::Graphics::GetInstance().GetDeviceContext()->RSSetViewports(1, &m_currentViewport);
			}
			/*
			[�֐��T�v]
			�������̃����_�[�^�[�Q�b�g��ݒ肷��

			[����]
			UINT			num		�����_�[�^�[�Q�b�g�̐�
			RenderTarget*	rtv[]	�����_�[�^�[�Q�b�g�̔z��
			DepthStencil*	dsv		�f�v�X�X�e���V��
			*/
			void RenderContext::SetRenderTargets(UINT num, std::shared_ptr<RenderTarget> rtv[], std::shared_ptr<DepthStencil> dsv)
			{
				static Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtvs[4];

				if (num > 4) num = 4;
				for (UINT i = 0; i < num; ++i)
					rtvs[i] = rtv[i]->GetView();
				graphics::Graphics::GetInstance().GetDeviceContext()->OMSetRenderTargets(num, rtvs->GetAddressOf(), dsv ? dsv->GetView().Get() : nullptr);

				SetViewport((float)rtv[0]->GetWidth(), (float)rtv[0]->GetHeight());
			}

			/*
			[�֐��T�v]
			�����_�[�^�[�Q�b�g�̃N���A

			[����]
			RenderTarget&		rtv		�����_�[�^�[�Q�b�g
			const float*		color	�N���A�J���[
			*/
			void RenderContext::ClearRenderTargetView(std::shared_ptr<RenderTarget> rtv, DirectX::XMFLOAT4 _color)
			{
				float color[] = { _color.x, _color.y, _color.z, _color.w };
				graphics::Graphics::GetInstance().GetDeviceContext()->ClearRenderTargetView(rtv->GetView().Get(), color);
			}

			/*
			[�֐��T�v]
			�f�v�X�X�e���V���̃N���A

			[����]
			DepthStencil&	dsv			�f�v�X�X�e���V��
			*/
			void RenderContext::ClearDepthStencilView(std::shared_ptr<DepthStencil> dsv)
			{
				graphics::Graphics::GetInstance().GetDeviceContext()->ClearDepthStencilView(dsv->GetView().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
			}

			/*
			[�֐��T�v]
			�C���f�b�N�X�t���v���~�e�B�u��`��

			[����]
			UINT	indexCount		�C���f�b�N�X�̐�
			*/
			void RenderContext::DrawIndexed(UINT indexCount)
			{
				graphics::Graphics::GetInstance().GetDeviceContext()->DrawIndexed(indexCount, 0, 0);
			}

			/*
			[�֐��T�v]
			�萔�o�b�t�@�i���[���h�}�g���N�X�j��ݒ肷��

			[����]
			DirectX::XMFLOAT4X4*	worldMat
			*/
			void RenderContext::SetWorldMatrix(DirectX::XMFLOAT4X4* worldMat)
			{
				DirectX::XMFLOAT4X4 world;
				DirectX::XMStoreFloat4x4(&world,
					DirectX::XMMatrixTranspose(
						DirectX::XMLoadFloat4x4(worldMat)
					)
				);
				graphics::Graphics::GetInstance().GetDeviceContext()->UpdateSubresource(m_pConstantBuffers[0], 0, NULL, &world, 0, 0);
			}

			/*
			[�֐��T�v]
			�萔�o�b�t�@�i�r���[�}�g���N�X�j��ݒ肷��

			[����]
			DirectX::XMFLOAT4X4*	viewMat
			*/
			void RenderContext::SetViewMatrix(DirectX::XMFLOAT4X4* viewMat)
			{
				DirectX::XMFLOAT4X4 view;
				DirectX::XMStoreFloat4x4(&view,
					DirectX::XMMatrixTranspose(
						DirectX::XMLoadFloat4x4(viewMat)
					)
				);
				graphics::Graphics::GetInstance().GetDeviceContext()->UpdateSubresource(m_pConstantBuffers[1], 0, NULL, &view, 0, 0);
			}

			/*
			[�֐��T�v]
			�萔�o�b�t�@�i�v���W�F�N�V�����}�g���N�X�j��ݒ肷��

			[����]
			DirectX::XMFLOAT4X4*	projectionMat
			*/
			void RenderContext::SetProjectionMatrix(DirectX::XMFLOAT4X4* projectionMat)
			{
				DirectX::XMFLOAT4X4 projection;
				DirectX::XMStoreFloat4x4(&projection,
					DirectX::XMMatrixTranspose(
						DirectX::XMLoadFloat4x4(projectionMat)
					)
				);
				graphics::Graphics::GetInstance().GetDeviceContext()->UpdateSubresource(m_pConstantBuffers[2], 0, NULL, &projection, 0, 0);
			}


			///*
			//[�֐��T�v]
			//�e�N�X�`����ݒ肷��

			//[����]
			//UINT						slot	���Ԗڂ�
			//ID3D11ShaderResourceView*	tex		�e�N�X�`��
			//EnKind						enKind	�ݒ肷��V�F�[�_�[�̎��
			//*/
			//void RenderContext::SetTexture(UINT slot, ID3D11ShaderResourceView* tex, Shader::EnKind enKind)
			//{
			//	if (slot < 0 || SHADER_RESOURCE::MAX_SHADER_RESOURCE < slot) {
			//		// �G���[�o��
			//		MessageBox(nullptr, "Error:SetTexture[Slot]", "Error", MB_ICONERROR | MB_OK);
			//		return;
			//	}

			//	m_shaderResources[slot] = tex;
			//	switch (enKind)
			//	{
			//	case Shader::enVertex:
			//	{
			//		g_graphics->GetContext()->VSSetShaderResources(slot, 1, &m_shaderResources[slot]);
			//		break;
			//	}
			//	case Shader::enPixel:
			//	{
			//		g_graphics->GetContext()->PSSetShaderResources(slot, 1, &m_shaderResources[slot]);
			//		break;
			//	}
			//	}
			//}


			/*
			[�֐��T�v]
			�J�����O���[�h��ݒ肷��

			[����]
			EnCullingMode		enCullMode		�J�����O���[�h
			*/
			void RenderContext::SetCullingMode(EnCullingMode enCullMode)
			{
				graphics::Graphics::GetInstance().GetDeviceContext()->RSSetState(m_pRasterizerState[enCullMode].Get());
			}

			/*
			[�֐��T�v]
			�f�v�X�������[�h��ݒ肷��

			[����]
			EnDepthMode		enDepthMode		�f�v�X���[�h
			*/
			void RenderContext::SetDepthMode(EnDepthMode enDepthMode)
			{
				graphics::Graphics::GetInstance().GetDeviceContext()->OMSetDepthStencilState(m_pDepthState[enDepthMode].Get(), NULL);
			}

			/*
			[�֐��T�v]
			�u�����h���[�h��ݒ肷��

			[����]
			EnBlendMode	enBlendMode	�u�����h���[�h
			*/
			void RenderContext::SetBlendMode(EnBlendMode enBlendMode)
			{
				float blendFactor[4] = { D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };
				graphics::Graphics::GetInstance().GetDeviceContext()->OMSetBlendState(m_pBlendState[enBlendMode].Get(), blendFactor, 0xffffffff);
			}

			/*
			[�֐��T�v]
			�T���v���[�X�e�[�g���[�h��ݒ肷��

			[����]
			EnSamplerStateMode	enSamplerStateMode		�T���v���[�X�e�[�g���[�h
			*/
			void RenderContext::SetSamplerStateMode(EnSamplerStateMode enSamplerStateMode)
			{
				graphics::Graphics::GetInstance().GetDeviceContext()->PSSetSamplers(0, 1, m_pSamplerState[enSamplerStateMode].GetAddressOf());
			}
		}
	}
}