module;

#include <Graphics/src/GraphicsHeader.h>

module FullscreenRenderer;

using namespace TR;

namespace FullscreenRenderer {

	void Init(_Context* fsRenderer, Long2 size, ID3D12GraphicsCommandList* cmdList)
	{
		fsRenderer->size = size;
		
		fsRenderer->viewPort = { 0.0f, 0.0f, (float)size[0], (float)size[1], 0.0f, 1.0f };
		fsRenderer->scissorRect = { 0, 0, size[0], size[1] };

		Graphics::VertexLayout::Init(&fsRenderer->vertexLayout, {
			{ "CORNER", DXGI_FORMAT_R32G32_FLOAT }
			});
		Graphics::VertexLayout::CreateDesc(&fsRenderer->vertexLayout);

		std::vector<Float2> vertices(6);
		for (UINT i = 0; i < 6; i++) {
			vertices[i][0] = (i >= 1 && i <= 3) ? 1.0f : -1.0f;
			vertices[i][1] = (i >= 2 && i <= 4) ? -1.0f : 1.0f;
		}

		fsRenderer->vertexBuffer.Init(6, sizeof(Float2));
		fsRenderer->vertexBuffer.Upload(&vertices[0], cmdList);
	}

	void Render(_Context* fsRenderer, Graphics::Renderer::_Context* renderer, Graphics::InputParameter::Map::_Context* inputMap, ID3D12GraphicsCommandList* cmdList)
	{
		Graphics::Renderer::Render(renderer, cmdList, fsRenderer->vertexBuffer.vertexBuffer.vbView, inputMap, fsRenderer->viewPort, fsRenderer->scissorRect);
	}

	void Init(_Context* fsRenderer, Graphics::Renderer::_Context* renderer, Graphics::InputParameter::Map::_Context* inputMap, Long2 size, ID3D12GraphicsCommandList* cmdList)
	{
		Init(fsRenderer, size, cmdList);
		renderer->vertexLayout = fsRenderer->vertexLayout;
		Graphics::Renderer::Init(renderer, inputMap);
	}

}

void _FullscreenRenderer::Init(Long2 size, ID3D12GraphicsCommandList* cmdList)
{
	FullscreenRenderer::Init(&fsRenderer, &renderer, &inputMap, size, cmdList);
}

void _FullscreenRenderer::Render(ID3D12GraphicsCommandList* cmdList)
{
	FullscreenRenderer::Render(&fsRenderer, &renderer, &inputMap, cmdList);
}