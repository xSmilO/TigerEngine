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
    for(GameObject *obj : m_Objects) {
        obj->OnMousePressed(button, mouseX, mouseY);
    }
}

void Scene::OnMouseReleased(int button, float mouseX, float mouseY) {
    for(GameObject *obj : m_Objects) {
        obj->OnMouseReleased(button, mouseX, mouseY);
    }
}
} // namespace TigerEngine