#include "Scene/SpriteAnimator.h"
#include "Scene/GameObject.h"
#include "Scene/SpriteRenderer.h"
#include <algorithm>
#include <stdexcept>
#include <utility>

namespace TigerEngine {
SpriteAnimation SpriteAnimator::Grid(int columns, int rows, int firstFrame,
                                     int frameCount, float secondsPerFrame, bool loop) {
    if(columns <= 0 || rows <= 0 || firstFrame < 0 || frameCount <= 0 ||
       firstFrame >= columns * rows || frameCount > columns * rows - firstFrame ||
       secondsPerFrame <= 0.0f)
        throw std::invalid_argument("Invalid sprite sheet grid or animation duration");

    SpriteAnimation animation;
    animation.SecondsPerFrame = secondsPerFrame;
    animation.Loop = loop;
    for(int i = 0; i < frameCount; ++i) {
        int index = firstFrame + i;
        int col = index % columns;
        int row = index / columns;
        const float u0 = float(col) / float(columns);
        const float u1 = float(col + 1) / float(columns);
        const float v0 = 1.0f - float(row + 1) / float(rows);
        const float v1 = 1.0f - float(row) / float(rows);
        animation.Frames.push_back({{u0, v0}, {u1, v1}});
    }
    return animation;
}

void SpriteAnimator::AddAnimation(const std::string& name, SpriteAnimation animation) {
    if(name.empty() || animation.Frames.empty() || animation.SecondsPerFrame <= 0.0f)
        throw std::invalid_argument("Animation needs a name, frames and positive frame duration");
    m_Animations.insert_or_assign(name, std::move(animation));
    if(name == m_CurrentName) {
        m_Frame = 0;
        m_Elapsed = 0.0f;
        ApplyFrame();
    }
}

bool SpriteAnimator::Play(const std::string& name, bool restart) {
    if(m_Animations.find(name) == m_Animations.end()) return false;
    if(!restart && m_CurrentName == name) return true;
    m_CurrentName = name;
    m_Frame = 0;
    m_Elapsed = 0.0f;
    m_Playing = true;
    ApplyFrame();
    return true;
}

void SpriteAnimator::Stop() { m_Playing = false; }

void SpriteAnimator::Update(float deltaTime) {
    if(!m_Playing || deltaTime <= 0.0f) return;
    auto it = m_Animations.find(m_CurrentName);
    if(it == m_Animations.end()) return;
    const auto& animation = it->second;
    m_Elapsed += deltaTime;
    // Arithmetic skipping avoids O(deltaTime/frameDuration) loops after a pause.
    int advanced = static_cast<int>(m_Elapsed / animation.SecondsPerFrame);
    if(advanced <= 0) return;
    m_Elapsed -= advanced * animation.SecondsPerFrame;
    const int count = static_cast<int>(animation.Frames.size());
    if(animation.Loop) m_Frame = (m_Frame + advanced) % count;
    else {
        m_Frame = std::min(m_Frame + advanced, count - 1);
        if(m_Frame == count - 1) m_Playing = false;
    }
    ApplyFrame();
}

void SpriteAnimator::ApplyFrame() {
    if(!Owner) return;
    auto* sprite = Owner->GetComponent<SpriteRenderer>();
    auto it = m_Animations.find(m_CurrentName);
    if(!sprite || it == m_Animations.end()) return;
    const auto& frame = it->second.Frames[m_Frame];
    sprite->SetUVRect(frame.UVMin, frame.UVMax);
}
} // namespace TigerEngine
