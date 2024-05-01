export module App.Scene.Camera;

export import TR.Essentials.Vec;

export namespace App::Scene::Camera {

	class Camera {

	public:

		TR::Float3 position = {};
		TR::Float2 rotation = {};
		float fov = 3.14f / 2.0f;
		float aspectRatio = 1.0f;

	};

}