#include "actions/EventAction.hh"

#include <G4Event.hh>
#include <G4RunManager.hh>

#include "utils/RootManager.hh"

namespace Test
{

EventAction::EventAction()
: G4UserEventAction()
{}

EventAction::~EventAction()
{}

void EventAction::BeginOfEventAction(const G4Event*)
{}

void EventAction::EndOfEventAction(const G4Event*)
{
  auto* rootManager = RootManager::Instance();
  
  // TODO: event info

  rootManager->Fill();
}

}