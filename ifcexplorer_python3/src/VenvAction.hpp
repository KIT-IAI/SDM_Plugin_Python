#pragma once

#include "ActionFeatureHelper.hpp"

class VenvAction : public sdm::plugin::ActionFeatureHelper
{
  public:
    VenvAction();

    void execute() const override;
};
