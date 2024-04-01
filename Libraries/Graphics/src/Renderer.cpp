module TR.Graphics.Renderer;

import TR.Graphics.Device;

namespace TR {

	namespace Graphics {

		namespace Renderer {

			void Init(_RendererPart* renderer, _InputMap* inputMap, _ShaderSet* shaderSet, _VertexLayout* vertexLayout)
			{
				HRESULT ret;

				D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
				desc.pRootSignature = inputMap->rootSignature.Get();
				desc.VS = GetBytecode(&shaderSet->vertex);
				desc.PS = GetBytecode(&shaderSet->pixel);
				desc.DS = GetBytecode(&shaderSet->domain);
				desc.HS = GetBytecode(&shaderSet->hull);
				desc.GS = GetBytecode(&shaderSet->geometry);
				desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
				desc.BlendState.RenderTarget[0].BlendEnable = false; // Implement later
				desc.SampleMask = 0xffffffff;
				desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
				desc.InputLayout = vertexLayout->layoutDesc;
				desc.PrimitiveTopologyType = renderer->topologyType;
				desc.NumRenderTargets = 1;
				desc.RTVFormats[0] = renderer->renderTargetFormat;
				desc.SampleDesc = { 1, 0 };
				desc.DepthStencilState.DepthEnable = false;
				desc.DSVFormat = DXGI_FORMAT_UNKNOWN;

				ret = device->CreateGraphicsPipelineState(&desc, __uuidof(ID3D12PipelineState), &renderer->pipelineState);
				if (ret != 0) {
					throw _FailedPipelineStateCreationException(ret);
				}
			}

			void Render(_RendererPart* renderer, _InputMap* inputMap, _VertexBuffer* vertexBuffer,
				ID3D12GraphicsCommandList* cmdList, D3D12_VIEWPORT viewPort, RECT scissorRect)
			{
				cmdList->SetPipelineState(renderer->pipelineState.Get());
				SetParameters(inputMap, cmdList);

				cmdList->RSSetViewports(1, &viewPort);
				cmdList->RSSetScissorRects(1, &scissorRect);
				cmdList->IASetPrimitiveTopology(renderer->topology);
				Set(vertexBuffer, cmdList);

				cmdList->DrawInstanced(vertexBuffer->vertexBuffer.vbView.SizeInBytes / vertexBuffer->vertexBuffer.vbView.StrideInBytes, 1, 0, 0);
			}

			void Init(_Renderer* renderer)
			{
				Init(&renderer->renderer, &renderer->inputMap, &renderer->shaderSet, &renderer->vertexLayout);
			}

			void Render(_Renderer* renderer, _VertexBuffer* vertexBuffer,
				ID3D12GraphicsCommandList* cmdList, D3D12_VIEWPORT viewPort, RECT scissorRect)
			{
				Render(&renderer->renderer, &renderer->inputMap, vertexBuffer, cmdList, viewPort, scissorRect);
			}

		}

	}

}