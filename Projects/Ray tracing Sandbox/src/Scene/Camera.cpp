module Scene.Camera;

import TR.Graphics.InputParameter;
import TR.Graphics.BufferResource;

namespace Scene::Camera {

	void Init(_Camera& camera, Graphics::Scene::RayTracer::_RayTracer& rayTracer)
	{
		Init(&rayTracer.fullRenderer.renderer.inputMap.arrays["camera"], 0);
		
		Init(&camera.buffer, 1, sizeof(_Camera::_Data));
		rayTracer.fullRenderer.renderer.inputMap.arrays["camera"].gpuAddress = GetGPUAddress(&camera.buffer.resource);
	}

	void Update(_Camera& camera, TR::_Graphics& graphics)
	{
		TR::UploadBuffer(&camera.buffer.upload, &camera.buffer.resource, &camera.data, graphics.cmdList.cmdList.Get());
	}

}