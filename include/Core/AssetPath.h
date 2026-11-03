#pragma once
#include <filesystem>

namespace TigerEngine {
// Relative paths resolve inside the asset folder beside the executable.
// Both "textures/player.png" and legacy "assets/textures/player.png" work.
// Absolute paths are used directly. Configure the root before loading resources.
class AssetPath {
public:
    static void SetRoot(const std::filesystem::path& directory);
    static const std::filesystem::path& GetRoot();
    // Returns an absolute, normalized path, including for missing files.
    static std::filesystem::path Resolve(const std::filesystem::path& path);
};
}
