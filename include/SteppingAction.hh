#pragma once

#include "G4UserSteppingAction.hh"

class G4Step;
class G4LogicalVolume;

namespace Test
{

class SteppingAction : public G4UserSteppingAction
{
  public:
    SteppingAction();
    ~SteppingAction();

    void UserSteppingAction(const G4Step*) override;

  private:
    G4LogicalVolume* fSipmLV = nullptr;
};

}