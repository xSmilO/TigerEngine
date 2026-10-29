#include "Scene/Scene.h"
#include "Graphics/Renderer2D.h"

namespace TigerEngine {
Scene::Scene() {
    m_ActiveCamera = &m_DefaultCamera;
}

Scene::~Scene() {
    for(GameObject *obj : m_Objects)
        delete obj;
    m_Objects.clear();
}

void Scene::AddObject(GameObject *object) {
    object->CurrentScene = this;
    m_Objects.push_back(object);
}

void Scene::Start() {
    for(GameObject *obj : m_Objects)
        obj->Start();
}

void Scene::Update(float deltaTime) {
    for(GameObject *obj : m_Objects)
        obj->Update(deltaTime);
}

void Scene::Draw() {
    Renderer2D::BeginScene(*m_ActiveCamera);

    for(GameObject *obj : m_Objects) {
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
    for(auto *obj : m_Objects) {
        for(auto *comp : obj->GetComponents())
            comp->OnGUI();
    }
}
} // namespace TigerEngine