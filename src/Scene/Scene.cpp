#include "Scene/Scene.h"

namespace TigerEngine {
Scene::Scene() {
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
    for(GameObject *obj : m_Objects)
        obj->Draw();
}
} // namespace TigerEngine