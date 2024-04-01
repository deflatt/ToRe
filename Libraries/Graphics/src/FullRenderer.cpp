module TR.Graphics.FullRenderer;

namespace TR {

	namespace Graphics {

		namespace FullRenderer {

			void Init(_Renderer* renderer, _FullRendererPart* fullRenderer, ID3D12GraphicsCommandList* cmdList)
			{
				Init(renderer);
				Init(&fullRenderer->vbuffer, 6, sizeof(Float2));
				
				fullRenderer->vertices = {
					{ -1.0f, 1.0f }, { 1.0f, 1.0f }, { -1.0f, -1.0f },
					{ 1.0f, -1.0f }, { -1.0f, -1.0f }, { 1.0f, 1.0f }
				};
				Upload(&fullRenderer->vbuffer, &fullRenderer->vertices[0], cmdList);
			}

			void Render(_Renderer* renderer, _FullRendererPart* fullRenderer, ID3D12GraphicsCommandList* cmdList, D3D12_VIEWPORT viewPort, RECT scissorRect)
			{
				Render(renderer, &fullRenderer->vbuffer, cmdList, viewPort, scissorRect);
			}

			void Init(_FullRenderer* fullRenderer, ID3D12GraphicsCommandList* cmdList)
			{
				Init(&fullRenderer->renderer, &fullRenderer->fullRenderer, cmdList);
			}

			void Render(_FullRenderer* fullRenderer, ID3D12GraphicsCommandList* cmdList, D3D12_VIEWPORT viewPort, RECT scissorRect)
			{
				Render(&fullRenderer->renderer, &fullRenderer->fullRenderer, cmdList, viewPort, scissorRect);
			}


		}

	}

}