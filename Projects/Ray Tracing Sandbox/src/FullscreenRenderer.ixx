module;

#include <Graphics/src/GraphicsHeader.h>

export module FullscreenRenderer;

export import TR.Graphics.Renderer;
export import TR.Essentials.Array;
export import TR.Graphics.VertexBuffer;
export import TR.Graphics.VertexLayout;
export import TR.Graphics.InputParameter;

using namespace TR;

export namespace FullscreenRenderer {

	struct _Context {
		Long2 size = {};
		Graphics::VertexLayout::_Context vertexLayout = {};
		Graphics::_VertexBuffer vertexBuffer = {};
		D3D12_VIEWPORT viewPort = {};
		RECT scissorRect = {};
	};

	void Init(_Context* fsRenderer, Long2 size, ID3D12GraphicsCommandList* cmdList);

	void Render(_Context* fsRenderer, Graphics::Renderer::_Context* renderer, Graphics::InputParameter::Map::_Context* inputMap, ID3D12GraphicsCommandList* cmdList);

	void Init(_Context* fsRenderer, Graphics::Renderer::_Context* renderer
		, Graphics::InputParameter::Map::_Context* inputMap, Long2 size, ID3D12GraphicsCommandList* cmdList);

}

export struct _FullscreenRenderer {

	Graphics::Renderer::_Context renderer = {};
	Graphics::InputParameter::Map::_Context inputMap = {};
	FullscreenRenderer::_Context fsRenderer = {};

	void Init(Long2 size, ID3D12GraphicsCommandList* cmdList);
	void Render(ID3D12GraphicsCommandList* cmdList);

};