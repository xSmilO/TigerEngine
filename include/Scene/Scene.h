#pragma once
#include "Scene/GameObject.h"
#include <vector>

namespace TigerEngine {
class Scene {
  public:
    Scene();
    ~Scene();

    void AddObject(GameObject *object);

    void Start();
    void Update(float deltaTime);
    void Draw();

  private:
    std::vector<GameObject *> m_Objects;
};
} // namespace TigerEngine