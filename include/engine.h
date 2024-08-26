#pragma once

#include <memory>

#include "Camera.h"
#include "Renderer.h"

#define DEFAULT_WINDOW_WIDTH 960
#define DEFAULT_WINDOW_HEIGHT 540
#define MOUSE_SENSITIVITY 0.1f
#define INSTANCES_PER_CHUNK 3000

namespace timothy
{
	class Engine {
	private:
		bool centerCursor = false;
		std::unique_ptr<timothy::Camera> m_camera;
		std::unique_ptr<timothy::Renderer> m_renderer;
		std::unique_ptr <std::vector< glm::vec3 >> m_offsets;
		std::unique_ptr<Shader> m_shader;
	public:
		Engine();
		~Engine();
		int Run();
	};
}