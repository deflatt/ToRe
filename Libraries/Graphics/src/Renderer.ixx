export module TR.Graphics.Renderer;

export import TR.Graphics.VertexLayout;
export import TR.Graphics.VertexBuffer;
export import TR.Graphics.Shader;
export import TR.Graphics.InputParameter;
export import TR.Graphics.Exception;
import <d3d12.h>;
import TR.Graphics.Definitions;

export namespace TR {

	namespace Graphics {

		namespace Renderer {

			struct _RendererPart {
				D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
				D3D12_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
				DXGI_FORMAT renderTargetFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

				ComPtr<ID3D12PipelineState> pipelineState = {};
			};

			struct _FailedPipelineStateCreationException : public _D3D12Exception {
				_FailedPipelineStateCreationException(HRESULT returnValue) : _D3D12Exception("Failed to create pipeline state.", returnValue) {}
			};

			void Init(_RendererPart* renderer, _InputMap* inputMap, _ShaderSet* shaderSet, _VertexLayout* vertexLayout);

			void Render(_RendererPart* renderer, _InputMap* inputMap, _VertexBuffer* vertexBuffer, 
				ID3D12GraphicsCommandList* cmdList, D3D12_VIEWPORT viewPort, RECT scissorRect);

			struct _Renderer {
				_RendererPart renderer = {};
				_InputMap inputMap = {};
				_ShaderSet shaderSet = {};
				_VertexLayout vertexLayout = {};
			};

			void Init(_Renderer* renderer);
			void Render(_Renderer* renderer, _VertexBuffer* vertexBuffer, 
				ID3D12GraphicsCommandList* cmdList, D3D12_VIEWPORT viewPort, RECT scissorRect);

		}
		using namespace Renderer;

	}
	using namespace Graphics;

}