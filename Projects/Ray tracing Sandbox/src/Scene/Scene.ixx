export module Scene;

export import Scene.Camera;

export namespace Scene {

	struct _Scene {

		Camera::_Camera camera = {};

	};

	void Init(_Scene& scene, Graphics::Scene::RayTracer::_RayTracer& rayTracer);

	void Update(_Scene& scene, TR::_Graphics& graphics);

}