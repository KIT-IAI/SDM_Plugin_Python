#pragma once

#include <string>
#include <vector>

#include "Population.hpp"

struct PluginFeatures
{
    std::string name;
    std::string popup;
};

struct PluginState
{
    std::vector<PluginFeatures> pluginFeatures;
    std::string featureName;

    bool executed = false;
};