export module App.Scene.World;

export import TR.Essentials.AABBTree;
export import TR.Essentials.Vec;

export namespace App::Scene::World {

	struct _Material {

		TR::Float3 emission = {};
		TR::Float3 reflection = {};

	};

	struct _World {

		TR::AABBTree<int, float, 3, 2> voxels = {};
		std::vector<_Material> materials = {};

	};

}