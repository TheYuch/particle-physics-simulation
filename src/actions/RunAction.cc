#include "actions/RunAction.hh"
#include "utils/RootManager.hh"

class G4Run;

namespace Test
{

RunAction::RunAction()
: G4UserRunAction()
{}

RunAction::~RunAction()
{}

void RunAction::BeginOfRunAction(const G4Run*)
{
  RootManager* rootManager = RootManager::Instance();
  rootManager->Open();
}

void RunAction::EndOfRunAction(const G4Run*)
{
  RootManager::Instance()->WriteAndClose();
}

}