#include "pch.h"
#include "Utils.h"
#include <filesystem>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN // Stops most intrusive windows.h #defines
#include <windows.h>
// Overrides remaining intrusive windows.h #defines
#define GetObject GetObject 
#define max max
#define min min
#endif
#include <stdlib.h>
#include <stdio.h>

std::filesystem::path Utils::GetExecutableDirectory() {
    std::filesystem::path path;
    char buffer[MAX_PATH];
    GetModuleFileNameA(nullptr, buffer, MAX_PATH);
    path = buffer;
    return path.parent_path(); // Returns the directory containing the executable
}

std::filesystem::path Utils::GetProfileDirectory() {
    char* profilePath = nullptr;
    size_t len;
    errno_t err = _dupenv_s(&profilePath, &len, "USERPROFILE");
    std::filesystem::path result;

    if (err == 0 && profilePath != nullptr) {
        result = std::filesystem::path(profilePath);
    } // else handle error if needed

    free(profilePath); // Safe to call even if profilePath is nullptr
    return result;
}