export module Scene.Camera;

export import TR.Essentials.Vec;
export import Graphics.Scene.RayTracer;
import TR.Graphics.ArrayResource;
import TR.Graphics.Graphics;

export namespace Scene::Camera {

	struct _Camera {

		struct _Data {
			TR::Float3 position = { 0.0f, 0.0f, 0.0f };
			TR::Float2 rotation = { 0.0f, 1.57f };
		} data = {};

		TR::_ArrayBuffer buffer = {};

	};

	void Init(_Camera& camera, Graphics::Scene::RayTracer::_RayTracer& rayTracer);

	void Update(_Camera& camera, TR::_Graphics& graphics);

}