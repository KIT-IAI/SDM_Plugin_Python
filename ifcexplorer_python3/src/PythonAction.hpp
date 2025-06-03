#pragma once

#include <LiveLogFeature.hpp>

#include "ActionFeatureHelper.hpp"
#include "AppFeatureHelper.hpp"
#include "IfcDBState.hpp"

class PythonAction : public sdm::plugin::ActionFeatureHelper
{
public:
    PythonAction(const std::string& name);
    PythonAction(const std::string& name, const std::string& popupName);
    ~PythonAction() override = default;

    void execute() const override;

    void setDb(IfcDB::Populationi* db) { m_state.db = db; }
    void setStates(IfcDB::utils::PopulationStates* states) { m_state.states = states; }
    void setAppFeature(sdm::plugin::AppFeatureHelper* appFeature) { m_appFeature = appFeature; }

    void setLogInterface(sdm::plugin::LiveLogInterface* logInterface)
    {
        m_logInterface = logInterface;
    }

private:
    IfcDBState m_state;
    sdm::plugin::LiveLogInterface* m_logInterface = nullptr;
    sdm::plugin::AppFeatureHelper* m_appFeature = nullptr;
};