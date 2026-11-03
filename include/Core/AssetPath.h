#pragma once
#include <filesystem>

namespace TigerEngine {

class AssetPath {
public:
    static void SetRoot(const std::filesystem::path& directory);
    static const std::filesystem::path& GetRoot();

    static std::filesystem::path Resolve(const std::filesystem::path& path);
};
}
