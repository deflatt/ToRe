export module Graphics.Scene.RayTracer;

export import TR.Graphics.Graphics;
export import Graphics.Window;
export import TR.Graphics.FullRenderer;

export namespace Graphics::Scene::RayTracer {

	struct _RayTracer {
		TR::_FullRenderer fullRenderer = {};
	};

	void Init(_RayTracer& rayTracer, TR::_Graphics& graphics);

	void Render(_RayTracer& rayTracer, Window::_Window& window);

}