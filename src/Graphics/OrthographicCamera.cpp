#include "Graphics/OrthographicCamera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace TigerEngine {
OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
    : m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f) {
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void OrthographicCamera::SetPosition(const glm::vec3 &position) {
    m_Position = position;
    RecalculateViewMatrix();
}

void OrthographicCamera::RecalculateViewMatrix() {
    // To move the camera forward, we actually move the entire world backward, hence the inverse transform
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position);
    m_ViewMatrix = glm::inverse(transform);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}
} // namespace TigerEngine