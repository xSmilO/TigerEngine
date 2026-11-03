#pragma once
#include "UI/UIElement.h"
#include "Graphics/Texture.h"
#include <functional>
#include <memory>
#include <utility>
namespace TigerEngine {
enum class HorizontalAlignment { Left, Center, Right };
enum class VerticalAlignment { Top, Center, Bottom };

class UIButton : public UIElement {
public:
    std::string Text;
    glm::vec4 Background{0.16f, 0.36f, 0.62f, 1.0f};
    glm::vec4 HoverBackground{0.25f, 0.48f, 0.75f, 1.0f};
    glm::vec4 PressBackground{0.11f, 0.27f, 0.46f, 1.0f};
    glm::vec4 TextColor{1.0f};
    // Shared handles keep textures alive after cache eviction.
    std::shared_ptr<const Texture> NormalTexture = nullptr;
    std::shared_ptr<const Texture> HoverTexture = nullptr;
    std::shared_ptr<const Texture> PressTexture = nullptr;
    glm::vec4 TextureTint{1.0f};
    // Source-image border sizes in pixels: left, right, top, bottom.
    // When enabled, corners stay fixed, edges stretch along one axis, center stretches both.
    bool UseNineSlice = false;
    glm::vec4 NineSliceBorders{0.0f};
    void SetNineSlice(float left, float right, float top, float bottom) {
        NineSliceBorders = {left, right, top, bottom};
        UseNineSlice = true;
    }
    void DisableNineSlice() { UseNineSlice = false; }
    void SetTexture(std::shared_ptr<const Texture> texture) { NormalTexture = texture; }
    void SetTextures(std::shared_ptr<const Texture> normal, std::shared_ptr<const Texture> hover = nullptr,
                     std::shared_ptr<const Texture> pressed = nullptr) {
        NormalTexture = normal;
        HoverTexture = hover;
        PressTexture = pressed;
    }
    void SetTextureTint(glm::vec4 tint) { TextureTint = tint; }

    std::function<void()> OnClick;
    bool Hovered = false, Pressed = false;
    HorizontalAlignment TextHorizontalAlignment = HorizontalAlignment::Center;
    VerticalAlignment TextVerticalAlignment = VerticalAlignment::Center;
    glm::vec2 TextPadding{12.0f, 8.0f};
    void SetTextAlignment(HorizontalAlignment horizontal, VerticalAlignment vertical) {
        TextHorizontalAlignment = horizontal;
        TextVerticalAlignment = vertical;
    }
    void SetTextPadding(glm::vec2 padding) { TextPadding = padding; }
    void SetOnClick(std::function<void()> callback) { OnClick = std::move(callback); }
    void Draw(const Font* font) const override;
};
} // namespace TigerEngine
