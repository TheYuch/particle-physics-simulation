#include "actions/SteppingAction.hh"

#include <G4Step.hh>
#include <G4Track.hh>
#include <G4StepPoint.hh>
#include <G4VProcess.hh>

SteppingAction::SteppingAction()
: G4UserSteppingAction()
{}

SteppingAction::~SteppingAction()
{}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  /* G4Track* track = step->GetTrack();
  const G4ParticleDefinition* particle = track->GetParticleDefinition();

  if (particle->GetPDGEncoding() == -22) // PDG encoding of -22 means particle is an optical photon
  {
    //G4StepPoint* pre = step->GetPreStepPoint();
    G4StepPoint* post = step->GetPostStepPoint();
    const G4VProcess* process = post->GetProcessDefinedStep();
    
    if (process)
    {
      G4String procName = process->GetProcessName();
      if (procName == "OpAbsorption")
      {
        // G4cout << "Optical absorption!" << G4endl;
      }
    }
  } */
}