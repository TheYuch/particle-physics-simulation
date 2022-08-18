#include "actions/ActionInitialization.hh"

#include "actions/PrimaryGeneratorAction.hh"
#include "actions/RunAction.hh"
#include "actions/EventAction.hh"
#include "actions/SteppingAction.hh"

namespace Test
{

ActionInitialization::ActionInitialization()
: G4VUserActionInitialization()
{}

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::BuildForMaster() const
{
  RunAction* runAction = new RunAction;
  SetUserAction(runAction);
}

void ActionInitialization::Build() const
{
  SetUserAction(new PrimaryGeneratorAction);
  SetUserAction(new RunAction);
  SetUserAction(new EventAction);
  SetUserAction(new SteppingAction);
}

}
