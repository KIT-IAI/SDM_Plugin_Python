#include <fstream>

#include "PythonPluginManifest.hpp"

PythonPluginManifest loadPythonPluginManifest(const fs::path& path)
{
    std::ifstream is{path};

    if (!is)
    {
        throw std::runtime_error("could not read manifest: " + path.string());
    }

    PythonPluginManifest manifest;

    std::string line;

    while (getline(is, line))
    {
        const auto index = line.find('=');

        if (index == std::string::npos)
        {
            continue;
        }

        const auto key = line.substr(0, index);
        const auto value = line.substr(1 + index);

        if (key == "ACTION")
        {
            manifest.actionName = value;
        }
        else if (key == "POPUP")
        {
            manifest.popupName = value;
        }
        else if (key == "SCRIPT")
        {
            manifest.scriptName = value;
        }
    }

    return manifest;
}

std::vector<PythonPluginManifest> loadManifests(const fs::path& rootDir)
{
    std::vector<PythonPluginManifest> scripts;

    for (fs::directory_iterator it{rootDir}; it != fs::directory_iterator{}; ++it)
    {
        if (!it->is_directory())
        {
            continue;
        }

        const auto manifestPath = it->path() / "python_plugin.manifest";

        if (!exists(manifestPath))
        {
            continue;
        }

        auto manifest = loadPythonPluginManifest(manifestPath);
        manifest.manifestDir = it->path();

        scripts.emplace_back(manifest);
    }

    return scripts;
}
