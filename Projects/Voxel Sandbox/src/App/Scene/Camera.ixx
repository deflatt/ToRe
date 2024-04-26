export module App.Scene.Camera;

export import TR.Essentials.Vec;

export namespace App::Scene::Camera {

	struct _Camera {

		TR::Float3 position = {};
		TR::Float2 rotation = { 0.0f, 3.14f / 2.0f };
		float fov = 3.14f / 2.0f;
		float aspectRatio = 1.0f;

	};

}