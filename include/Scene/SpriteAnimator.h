#pragma once
#include "Core/Component.h"
#include "Core/Math.h"
#include <string>
#include <unordered_map>
#include <vector>

namespace TigerEngine {
class SpriteRenderer;

struct SpriteFrame {
    Vector2 UVMin{0.0f, 0.0f};
    Vector2 UVMax{1.0f, 1.0f};
};

struct SpriteAnimation {
    std::vector<SpriteFrame> Frames;
    float SecondsPerFrame = 0.1f;
    bool Loop = true;
};

class SpriteAnimator : public Component {
  public:

    static SpriteAnimation Grid(int columns, int rows, int firstFrame,
                                int frameCount, float secondsPerFrame,
                                bool loop = true);

    void AddAnimation(const std::string& name, SpriteAnimation animation);
    bool Play(const std::string& name, bool restart = false);
    void Stop();
    void Update(float deltaTime) override;
    const std::string& GetCurrentAnimation() const { return m_CurrentName; }
    int GetFrameIndex() const { return m_Frame; }

  private:
    void ApplyFrame();
    std::unordered_map<std::string, SpriteAnimation> m_Animations;
    std::string m_CurrentName;
    int m_Frame = 0;
    float m_Elapsed = 0.0f;
    bool m_Playing = false;
};
}
