module App;

import Graphics;

import TR.Graphics.Device;
import TR.Graphics.Debugging;

namespace App {

	void Init(_App& app)
	{
		TR::EnableDebugger();
		TR::InitDevice();

		Scene::Init(app.scene, app.graphics.scene.rayTracer);
		Graphics::Init(app.graphics, app.scene);
	}

	void Update(_App& app) 
	{
		Scene::Update(app.scene, app.graphics.window.windowGraphics.graphics);
		Graphics::Update(app.graphics, app.scene);
	}

}