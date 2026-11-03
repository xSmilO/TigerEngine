#include "Scene/Scene.h"
#include "Graphics/Renderer2D.h"

namespace TigerEngine {
Scene::Scene() {
    m_ActiveCamera = &m_DefaultCamera;
}

Scene::~Scene() {
    Clear();
}

GameObject* Scene::AddObject(std::unique_ptr<GameObject> object) {
    if (!object) return nullptr;
    auto* borrowed = object.get();
    object->CurrentScene = this;
    m_Objects.push_back(std::move(object));
    return borrowed;
}

void Scene::Clear() {
    m_ActiveCamera = &m_DefaultCamera;
    // Clear scene links before destructors run; objects detach hierarchy links.
    for (const auto& obj : m_Objects) obj->CurrentScene = nullptr;
    m_Objects.clear();
}

void Scene::Start() {
    for(const auto& obj : m_Objects)
        obj->Start();
}

void Scene::Update(float deltaTime) {
    for(const auto& obj : m_Objects)
        obj->Update(deltaTime);
}

void Scene::Draw() {
    Renderer2D::BeginScene(*m_ActiveCamera);

    for(const auto& obj : m_Objects) {
        obj->Draw();
    }

    Renderer2D::EndScene();
}

void Scene::OnMousePressed(int button, float mouseX, float mouseY) {
}

void Scene::OnMouseReleased(int button, float mouseX, float mouseY) {
}

void Scene::OnWindowResized(int width, int height) {
    if(height == 0)
        return; // Prevent division by zero if the window is minimized

    float aspectRatio = (float)width / (float)height;

    float zoom = 0.9f;

    // Update the fallback camera
    m_DefaultCamera.SetProjection(-aspectRatio * zoom, aspectRatio * zoom, -zoom, zoom);

    // If the user plugged in a custom camera, update it too
    if(m_ActiveCamera && m_ActiveCamera != &m_DefaultCamera) {
        m_ActiveCamera->SetProjection(-aspectRatio * zoom, aspectRatio * zoom, -zoom, zoom);
    }
}

void Scene::OnGUI() {
    for(const auto& obj : m_Objects) {
        for(auto *comp : obj->GetComponents())
            comp->OnGUI();
    }
}
} // namespace TigerEngine
