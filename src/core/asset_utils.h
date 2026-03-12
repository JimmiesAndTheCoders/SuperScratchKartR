#ifndef ASSET_UTILS_H
#define ASSET_UTILS_H

#include <string>
#include <filesystem>

namespace fs = std::filesystem;

inline std::string ResolveAssetPath(const std::string& relativePath) {
    fs::path currentDir = fs::current_path();
    for (int i = 0; i < 4; ++i) {
        fs::path attempt = currentDir / relativePath;
        if (fs::exists(attempt)) return attempt.string();
        if (currentDir.has_parent_path()) currentDir = currentDir.parent_path();
        else break;
    }
    return relativePath;
}

#endif