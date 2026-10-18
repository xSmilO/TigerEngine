#pragma once

namespace TigerEngine {
class Texture;

class GameObject {
  public:
    GameObject();
    virtual ~GameObject() = default;

    virtual void Start() {
    }
    virtual void Update(float deltaTime) {
    }
    virtual void Draw();

    float X, Y;
    float Width, Height;
    Texture *Sprite;
};
} // namespace TigerEngine