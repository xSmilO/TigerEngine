#include "Core/AssetPath.h"
#include <stdexcept>
#include <vector>
#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#endif

namespace TigerEngine {
namespace {
std::filesystem::path DefaultRoot() {
#if defined(_WIN32)
    std::vector<wchar_t> buffer(32768);
    const auto size = GetModuleFileNameW(nullptr, buffer.data(), static_cast<DWORD>(buffer.size()));
    if (size && size < buffer.size())
        return std::filesystem::path(std::wstring(buffer.data(), size)).parent_path() / "assets";
#elif defined(__linux__)
    std::error_code error;
    auto executable = std::filesystem::read_symlink("/proc/self/exe", error);
    if (!error) return executable.parent_path() / "assets";
#elif defined(__APPLE__)
    uint32_t size = 0;
    _NSGetExecutablePath(nullptr, &size);
    std::vector<char> buffer(size);
    if (_NSGetExecutablePath(buffer.data(), &size) == 0)
        return std::filesystem::weakly_canonical(buffer.data()).parent_path() / "assets";
#endif
    return std::filesystem::current_path() / "assets";
}
std::filesystem::path& Root() {
    static auto root = std::filesystem::weakly_canonical(DefaultRoot());
    return root;
}
}
void AssetPath::SetRoot(const std::filesystem::path& directory) {
    if (directory.empty()) throw std::invalid_argument("Asset root must not be empty");
    Root() = std::filesystem::weakly_canonical(std::filesystem::absolute(directory));
}
const std::filesystem::path& AssetPath::GetRoot() { return Root(); }
std::filesystem::path AssetPath::Resolve(const std::filesystem::path& path) {
    if (path.empty()) throw std::invalid_argument("Asset path must not be empty");
    if (path.is_absolute()) return std::filesystem::weakly_canonical(path);
    auto relative = path.lexically_normal();
    if (*relative.begin() == "assets") relative = relative.lexically_relative("assets");
    return std::filesystem::weakly_canonical(Root() / relative);
}
}
