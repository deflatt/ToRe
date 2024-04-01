export module TR.Graphics.FullRenderer;

import TR.Graphics.Renderer;
import TR.Graphics.VertexBuffer;
import TR.Essentials.Vec;
import <array>;

export namespace TR {

	namespace Graphics {

		namespace FullRenderer {

			struct _FullRendererPart {
				_VertexBuffer vbuffer = {};
				std::vector<Float2> vertices = {};
			};

			void Init(_Renderer* renderer, _FullRendererPart* fullRenderer, ID3D12GraphicsCommandList* cmdList);
			void Render(_Renderer* renderer, _FullRendererPart* fullRenderer, ID3D12GraphicsCommandList* cmdList, D3D12_VIEWPORT viewPort, RECT scissorRect);

			struct _FullRenderer {
				_Renderer renderer = {};
				_FullRendererPart fullRenderer = {};
			};

			void Init(_FullRenderer* fullRenderer, ID3D12GraphicsCommandList* cmdList);
			void Render(_FullRenderer* fullRenderer, ID3D12GraphicsCommandList* cmdList, D3D12_VIEWPORT viewPort, RECT scissorRect);

		}
		using namespace FullRenderer;

	}
	using namespace Graphics;

}