#pragma once

#include "sprite.h"
#include <glm/glm.hpp>

namespace cabbage
{
	struct Vertex
	{
		glm::vec3 position;
		UVCoordinate uvCoordinate;
		Vertex(glm::vec3 pos, UVCoordinate coordinate)
	  : position(pos)
		, uvCoordinate(coordinate)
		{}
	};
}
