#pragma once

#include <G4UserEventAction.hh>
#include "globals.hh"

namespace Test
{

class EventAction : public G4UserEventAction
{
  public:
    EventAction();
    ~EventAction();

    void BeginOfEventAction(const G4Event* event) override;
    void EndOfEventAction(const G4Event* event) override;
};

}