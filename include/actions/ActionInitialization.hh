#pragma once

#include <G4VUserActionInitialization.hh>

class ActionInitialization : public G4VUserActionInitialization
{
  public:
    ActionInitialization();
    ~ActionInitialization();

    void BuildForMaster() const override;
    void Build() const override;
};