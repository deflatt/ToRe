module;

#include "GraphicsHeader.h"

module TR.Graphics.Renderer;

namespace TR::Graphics {

	namespace Renderer {

		void Init(_Context* renderer, ID3D12RootSignature* rootSignature)
		{
			HRESULT ret;

			D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
			desc.pRootSignature = rootSignature;
			desc.VS = renderer->shaderSet.vertex;
			desc.PS = renderer->shaderSet.pixel;
			desc.DS = renderer->shaderSet.domain;
			desc.HS = renderer->shaderSet.hull;
			desc.GS = renderer->shaderSet.geometry;
			desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
			desc.BlendState.RenderTarget[0].BlendEnable = false; // Implement later
			desc.SampleMask = 0xffffffff;
			desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
			desc.InputLayout = renderer->vertexLayout.layoutDesc;
			desc.PrimitiveTopologyType = renderer->topologyType;
			desc.NumRenderTargets = 1;
			desc.RTVFormats[0] = renderer->renderTargetFormat;
			desc.SampleDesc = { 1, 0 };
			desc.DepthStencilState.DepthEnable = false;
			desc.DSVFormat = DXGI_FORMAT_UNKNOWN;

			ret = device->CreateGraphicsPipelineState(&desc, __uuidof(ID3D12PipelineState), &renderer->pipelineState);
			if (ret != 0) {
				throw E_FailedPipelineStateCreation(ret);
			}
		}

		void Render(_Context* renderer, ID3D12GraphicsCommandList* cmdList, D3D12_VERTEX_BUFFER_VIEW vertexBuffer, InputParameter::Map::_Context* inputMap, D3D12_VIEWPORT viewPort, RECT scissorRect)
		{
			cmdList->SetPipelineState(renderer->pipelineState.Get());
			if (inputMap)
				InputParameter::Map::SetParameters(inputMap, cmdList);

			cmdList->RSSetViewports(1, &viewPort);
			cmdList->RSSetScissorRects(1, &scissorRect);
			cmdList->IASetPrimitiveTopology(renderer->topology);
			cmdList->IASetVertexBuffers(0, 1, &vertexBuffer);

			cmdList->DrawInstanced(vertexBuffer.SizeInBytes / vertexBuffer.StrideInBytes, 1, 0, 0);
		}

	}

	void _Renderer::Init()
	{
		VertexLayout::CreateDesc(&renderer.vertexLayout);
		InputParameter::Map::Init(&inputMap);
		Renderer::Init(&renderer, inputMap.rootSignature.Get());
	}

	void TR::Graphics::_Renderer::Render(ID3D12GraphicsCommandList* cmdList, D3D12_VERTEX_BUFFER_VIEW vertexBuffer, D3D12_VIEWPORT viewPort, RECT scissorRect)
	{
		Renderer::Render(&renderer, cmdList, vertexBuffer, &inputMap, viewPort, scissorRect);
	}

}