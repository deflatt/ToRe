export module App.GUI.Scene.Renderer;

export import TR.Graphics.FullRenderer;
export import App.GUI.Window.Graphics;

export namespace App::GUI::Scene::Renderer {

	struct _Renderer {

		TR::_FullRenderer renderer = {};

		Window::Graphics::_Graphics* graphics = {};

	};

	void Init(_Renderer& renderer, Window::Graphics::_Graphics& graphics) {
		renderer = {};
		renderer.graphics = &graphics;
		{
			TR::_Compiler compiler = {};
			compiler.profile = "vs_6_6";
			TR::Compile(&renderer.renderer.renderer.shaderSet.vertex, "src/App/GUI/Scene/Renderer/RayVS.hlsl", compiler);
		}
		{
			TR::_Compiler compiler = {};
			compiler.profile = "ps_6_6";
			TR::Compile(&renderer.renderer.renderer.shaderSet.pixel, "src/App/GUI/Scene/Renderer/RayPS.hlsl", compiler);
		}
		TR::Init(&renderer.renderer.renderer.vertexLayout, { { "POSITION", DXGI_FORMAT_R32G32_FLOAT } });
	}

	void FinishInit(_Renderer& renderer) {
		TR::Init(&renderer.renderer.renderer.inputMap);
		TR::Init(&renderer.renderer, renderer.graphics->graphics.graphics.cmdList.cmdList.Get());
	}

	void Render(_Renderer& renderer) {
		TR::Int2 size = renderer.graphics->size;
		RECT scissorRect = { 0, 0, size[0], size[1] };
		D3D12_VIEWPORT viewPort = { 0.0f, 0.0f, (float)size[0], (float)size[1], 0.0f, 1.0f };
		TR::Render(&renderer.renderer, renderer.graphics->graphics.graphics.cmdList.cmdList.Get(), viewPort, scissorRect);
	}

}