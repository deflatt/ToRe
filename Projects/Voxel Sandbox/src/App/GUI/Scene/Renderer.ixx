export module App.GUI.Scene.Renderer;

export import TR.Graphics.FullRenderer;
export import App.GUI.Window.Graphics;

export namespace App::GUI::Scene::Renderer {

	class Renderer {

	public:

		void Init() {
			renderer = {};
			{
				TR::_Compiler compiler = {};
				compiler.profile = "vs_6_6";
				TR::Compile(&renderer.renderer.shaderSet.vertex, "src/App/GUI/Scene/Renderer/RayVS.hlsl", compiler);
			}
			{
				TR::_Compiler compiler = {};
				compiler.profile = "ps_6_6";
				TR::Compile(&renderer.renderer.shaderSet.pixel, "src/App/GUI/Scene/Renderer/RayPS.hlsl", compiler);
			}
			TR::Init(&renderer.renderer.vertexLayout, { { "POSITION", DXGI_FORMAT_R32G32_FLOAT } });
		}

		void FinishInit() {
			TR::Init(&renderer.renderer.inputMap);
			TR::Init(&renderer, graphics->GetCommandList());
		}

		void Render() {
			TR::Int2 size = graphics->size;
			RECT scissorRect = { 0, 0, size[0], size[1] };
			D3D12_VIEWPORT viewPort = { 0.0f, 0.0f, (float)size[0], (float)size[1], 0.0f, 1.0f };
			TR::Render(&renderer, graphics->GetCommandList(), viewPort, scissorRect);
		}

		TR::_FullRenderer renderer = {};

		Window::Graphics::Graphics* graphics = {};

	};

}