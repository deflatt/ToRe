module;

#include "GraphicsHeader.h"

module TR.Graphics.Renderer;

namespace TR::Graphics {

	namespace Renderer {

		void Init(_Context* context)
		{
			HRESULT ret;

			std::vector<D3D12_ROOT_PARAMETER> rootParameters(context->inputParameters.size());
			for (UINT i = 0; i < context->inputParameters.size(); i++) {
				rootParameters[i] = context->inputParameters[i].parameter;
			}

			CD3DX12_ROOT_SIGNATURE_DESC rootDesc = {};
			rootDesc.Init((UINT)rootParameters.size(), &rootParameters[0], (UINT)context->staticSamplers.size()
				, &context->staticSamplers[0], D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

			ComPtr<ID3DBlob> signatureBlob = {};
			ret = D3D12SerializeRootSignature(&rootDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, nullptr);
			if (ret != 0) {
				throw E_FailedRootSignatureSerialization(ret);
			}

			ret = device->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), &context->rootSignature);
			if (ret != 0) {
				throw E_FailedRootSignatureCreation(ret);
			}

			D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
			desc.pRootSignature = context->rootSignature.Get();
			desc.VS = context->shaderSet.vertex;
			desc.PS = context->shaderSet.pixel;
			desc.DS = context->shaderSet.domain;
			desc.HS = context->shaderSet.hull;
			desc.GS = context->shaderSet.geometry;
			desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
			desc.BlendState.RenderTarget[0].BlendEnable = false; // Implement later
			desc.SampleMask = 0xffffffff;
			desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
			desc.InputLayout = context->vertexLayout.layoutDesc;
			desc.PrimitiveTopologyType = context->topologyType;
			desc.NumRenderTargets = 1;
			desc.RTVFormats[0] = context->renderTargetFormat;
			desc.SampleDesc = { 1, 0 };
			desc.DepthStencilState.DepthEnable = false;
			desc.DSVFormat = DXGI_FORMAT_UNKNOWN;

			ret = device->CreateGraphicsPipelineState(&desc, __uuidof(ID3D12PipelineState), &context->pipelineState);
			if (ret != 0) {
				throw E_FailedPipelineStateCreation(ret);
			}
		}

	}

}