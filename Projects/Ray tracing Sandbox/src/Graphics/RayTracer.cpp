module Graphics.Scene.RayTracer;

import TR.Graphics.Shader;

namespace Graphics::Scene::RayTracer {

	void Init(_RayTracer& rayTracer, TR::_Graphics& graphics)
	{
		using namespace TR;
		{
			_Compiler compiler = {};
			compiler.profile = "vs_6_6";
			Compile(&rayTracer.fullRenderer.renderer.shaderSet.vertex, "src/Shaders/RayVS.hlsl", compiler);
		}
		{
			_Compiler compiler = {};
			compiler.profile = "ps_6_6";
			Compile(&rayTracer.fullRenderer.renderer.shaderSet.pixel, "src/Shaders/RayPS.hlsl", compiler);
		}
		Init(&rayTracer.fullRenderer.renderer.vertexLayout, { { "POSITION", DXGI_FORMAT_R32G32_FLOAT }});

		Init(&rayTracer.fullRenderer.renderer.inputMap);
		Init(&rayTracer.fullRenderer, graphics.cmdList.cmdList.Get());
	}

	void Render(_RayTracer& rayTracer, Window::_Window& window)
	{
		using namespace TR;

		Int2 windowSize = window.windowSize;
		RECT scissorRect = { 0, 0, windowSize[0], windowSize[1] };
		D3D12_VIEWPORT viewPort = { 0.0f, 0.0f, (float)windowSize[0], (float)windowSize[1], 0.0f, 1.0f };
		
		Render(&rayTracer.fullRenderer, window.windowGraphics.graphics.cmdList.cmdList.Get(), viewPort, scissorRect);
	}

}