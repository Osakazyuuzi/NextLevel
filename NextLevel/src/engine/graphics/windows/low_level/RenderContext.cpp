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

				//=== カリング設定
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
						// エラー出力
						MessageBox(nullptr, "Error:InitCullingMode", "Error", MB_ICONERROR | MB_OK);
						// 処理を中断
						return;
					}
				}
				SetCullingMode(enCULLING_NONE);

				//=== 深度ステート
				D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
				depthStencilDesc.DepthEnable = TRUE;
				depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
				depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
				depthStencilDesc.StencilEnable = FALSE;
				graphics::Graphics::GetInstance().GetDevice()->CreateDepthStencilState(&depthStencilDesc, &m_pDepthState[enDEPTH_ON]);//深度有効ステート
				depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
				graphics::Graphics::GetInstance().GetDevice()->CreateDepthStencilState(&depthStencilDesc, &m_pDepthState[enDEPTH_OFF]);//深度無効ステート
				SetDepthMode(enDEPTH_ON);

				//=== アルファブレンディング
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
						// エラー出力
						MessageBox(nullptr, "Error:InitBlendMode", "Error", MB_ICONERROR | MB_OK);
						// 処理を中断
						return;
					}
				}
				SetBlendMode(enBLEND_ALPHA);

				//=== サンプラー
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
						// エラー出力
						MessageBox(nullptr, "Error:InitSamplerStateMode", "Error", MB_ICONERROR | MB_OK);
						// 処理を中断
						return;
					}
				}
				SetSamplerStateMode(enSAMPLER_LINEAR);

				//=== 定数バッファ作成
				D3D11_BUFFER_DESC bufferDesc{};
				bufferDesc.ByteWidth = sizeof(DirectX::XMFLOAT4X4);
				bufferDesc.Usage = D3D11_USAGE_DEFAULT;
				bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
				bufferDesc.CPUAccessFlags = 0;
				bufferDesc.MiscFlags = 0;
				bufferDesc.StructureByteStride = sizeof(float);

				// ワールドマトリクス
				graphics::Graphics::GetInstance().GetDevice()->CreateBuffer(&bufferDesc, NULL, &m_pConstantBuffers[0]);
				graphics::Graphics::GetInstance().GetDeviceContext()->VSSetConstantBuffers(0, 1, &m_pConstantBuffers[0]);

				// ビューマトリクス
				graphics::Graphics::GetInstance().GetDevice()->CreateBuffer(&bufferDesc, NULL, &m_pConstantBuffers[1]);
				graphics::Graphics::GetInstance().GetDeviceContext()->VSSetConstantBuffers(1, 1, &m_pConstantBuffers[1]);

				// プロジェクションマトリクス
				graphics::Graphics::GetInstance().GetDevice()->CreateBuffer(&bufferDesc, NULL, &m_pConstantBuffers[2]);
				graphics::Graphics::GetInstance().GetDeviceContext()->VSSetConstantBuffers(2, 1, &m_pConstantBuffers[2]);

				//// サイズを設定
				//bufferDesc.ByteWidth = sizeof(Material);

				//// マテリアル
				//g_graphics->GetDevice()->CreateBuffer(&bufferDesc, NULL, &m_constantBuffers[3]);
				//g_graphics->GetContext()->VSSetConstantBuffers(3, 1, &m_constantBuffers[3]);
				//g_graphics->GetContext()->PSSetConstantBuffers(3, 1, &m_constantBuffers[3]);
			}

			/*
			[関数概要]
			終了処理
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
			//[関数概要]
			//頂点バッファを設定する

			//[引数]
			//ID3D11Buffer*	vtxBuf	頂点バッファ
			//*/
			//void RenderContext::SetVertexBuffer(ID3D11Buffer* vtxBuf, UINT stride)
			//{
			//	UINT offset = 0;
			//	g_graphics->GetContext()->IASetVertexBuffers(0, 1, &vtxBuf, &stride, &offset);
			//}

			///*
			//[関数概要]
			//インデックスバッファを設定する

			//[引数]
			//ID3D11Buffer*	idxBuf	インデックスバッファ
			//*/
			//void RenderContext::SetIndexBuffer(ID3D11Buffer* idxBuf)
			//{
			//	g_graphics->GetContext()->IASetIndexBuffer(idxBuf, DXGI_FORMAT_R32_UINT, 0);
			//}


			/*
			[関数概要]
			プリミティブのトポロジーを設定する

			[引数]
			D3D11_PRIMITIVE_TOPOLOGY	topology	トポロジータイプ
			*/
			void RenderContext::SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology)
			{
				graphics::Graphics::GetInstance().GetDeviceContext()->IASetPrimitiveTopology(topology);
			}

			/*
			[関数概要]
			ビューポートを設定する

			[引数]
			float width		横幅
			float height	縦幅
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
			[関数概要]
			複数枚のレンダーターゲットを設定する

			[引数]
			UINT			num		レンダーターゲットの数
			RenderTarget*	rtv[]	レンダーターゲットの配列
			DepthStencil*	dsv		デプスステンシル
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
			[関数概要]
			レンダーターゲットのクリア

			[引数]
			RenderTarget&		rtv		レンダーターゲット
			const float*		color	クリアカラー
			*/
			void RenderContext::ClearRenderTargetView(std::shared_ptr<RenderTarget> rtv, DirectX::XMFLOAT4 _color)
			{
				float color[] = { _color.x, _color.y, _color.z, _color.w };
				graphics::Graphics::GetInstance().GetDeviceContext()->ClearRenderTargetView(rtv->GetView().Get(), color);
			}

			/*
			[関数概要]
			デプスステンシルのクリア

			[引数]
			DepthStencil&	dsv			デプスステンシル
			*/
			void RenderContext::ClearDepthStencilView(std::shared_ptr<DepthStencil> dsv)
			{
				graphics::Graphics::GetInstance().GetDeviceContext()->ClearDepthStencilView(dsv->GetView().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
			}

			/*
			[関数概要]
			インデックス付きプリミティブを描画

			[引数]
			UINT	indexCount		インデックスの数
			*/
			void RenderContext::DrawIndexed(UINT indexCount)
			{
				graphics::Graphics::GetInstance().GetDeviceContext()->DrawIndexed(indexCount, 0, 0);
			}

			/*
			[関数概要]
			定数バッファ（ワールドマトリクス）を設定する

			[引数]
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
			[関数概要]
			定数バッファ（ビューマトリクス）を設定する

			[引数]
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
			[関数概要]
			定数バッファ（プロジェクションマトリクス）を設定する

			[引数]
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
			//[関数概要]
			//テクスチャを設定する

			//[引数]
			//UINT						slot	何番目か
			//ID3D11ShaderResourceView*	tex		テクスチャ
			//EnKind						enKind	設定するシェーダーの種類
			//*/
			//void RenderContext::SetTexture(UINT slot, ID3D11ShaderResourceView* tex, Shader::EnKind enKind)
			//{
			//	if (slot < 0 || SHADER_RESOURCE::MAX_SHADER_RESOURCE < slot) {
			//		// エラー出力
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
			[関数概要]
			カリングモードを設定する

			[引数]
			EnCullingMode		enCullMode		カリングモード
			*/
			void RenderContext::SetCullingMode(EnCullingMode enCullMode)
			{
				graphics::Graphics::GetInstance().GetDeviceContext()->RSSetState(m_pRasterizerState[enCullMode].Get());
			}

			/*
			[関数概要]
			デプス処理モードを設定する

			[引数]
			EnDepthMode		enDepthMode		デプスモード
			*/
			void RenderContext::SetDepthMode(EnDepthMode enDepthMode)
			{
				graphics::Graphics::GetInstance().GetDeviceContext()->OMSetDepthStencilState(m_pDepthState[enDepthMode].Get(), NULL);
			}

			/*
			[関数概要]
			ブレンドモードを設定する

			[引数]
			EnBlendMode	enBlendMode	ブレンドモード
			*/
			void RenderContext::SetBlendMode(EnBlendMode enBlendMode)
			{
				float blendFactor[4] = { D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };
				graphics::Graphics::GetInstance().GetDeviceContext()->OMSetBlendState(m_pBlendState[enBlendMode].Get(), blendFactor, 0xffffffff);
			}

			/*
			[関数概要]
			サンプラーステートモードを設定する

			[引数]
			EnSamplerStateMode	enSamplerStateMode		サンプラーステートモード
			*/
			void RenderContext::SetSamplerStateMode(EnSamplerStateMode enSamplerStateMode)
			{
				graphics::Graphics::GetInstance().GetDeviceContext()->PSSetSamplers(0, 1, m_pSamplerState[enSamplerStateMode].GetAddressOf());
			}
		}
	}
}