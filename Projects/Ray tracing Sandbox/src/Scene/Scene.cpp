module Scene;

namespace Scene {

	void Init(_Scene& scene, Graphics::Scene::RayTracer::_RayTracer& rayTracer)
	{
		Camera::Init(scene.camera, rayTracer);
	}

	void Update(_Scene& scene, TR::_Graphics& graphics)
	{
		Camera::Update(scene.camera, graphics);
	}

}