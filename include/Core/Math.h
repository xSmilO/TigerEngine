#pragma once
#include "Core/Point2D.h"
#include <glm/glm.hpp>

namespace TigerEngine {
// Alias GLM types to our own engine types
using Vector2 = Point2D;
using Vector3 = glm::vec3;
using Vector4 = glm::vec4;
using Matrix4 = glm::mat4;
} // namespace TigerEngine
