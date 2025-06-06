#pragma once

#include "ActionFeatureHelper.hpp"
#include "PythonPluginManifest.hpp"
#include <ifcdb/Population.h>
#include <ifcdb/utils/PopulationObserver.hpp>
#include "LiveLogFeature.hpp"
#include "AppFeatureHelper.hpp"
#include "IfcDBState.hpp"

class ManifestAction : public sdm::plugin::ActionFeatureHelper
{
  public:
    explicit ManifestAction(const PythonPluginManifest& manifest);

    void execute() const override;

    void setDb(IfcDB::Populationi* db) { m_state.db = db; }
    void setStates(IfcDB::utils::PopulationStates* states) { m_state.states = states; }
    void setAppFeature(sdm::plugin::AppFeatureHelper* appFeature) { m_appFeature = appFeature; }

    void setLogInterface(sdm::plugin::LiveLogInterface* logInterface)
    {
        m_logInterface = logInterface;
    }

  private:
    PythonPluginManifest m_manifest;

    IfcDBState m_state;
    sdm::plugin::LiveLogInterface* m_logInterface = nullptr;
    sdm::plugin::AppFeatureHelper* m_appFeature = nullptr;
};
