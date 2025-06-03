#pragma once

#include <string>
#include <vector>

#include "fs.hpp"

struct PythonPluginManifest
{
    std::string actionName;
    std::string popupName;
    std::string scriptName;
    fs::path manifestDir;
};

std::vector<PythonPluginManifest> loadManifests(const fs::path& rootDir);
