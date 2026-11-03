#pragma once
#include <glm/vec2.hpp>

namespace TigerEngine {
// Floating-point (x, y) coordinates; the API using the point defines its space.
// Supports construction Point2D{x, y}, arithmetic and direct .x/.y access.
using Point2D = glm::vec2;
}
