#include "SteppingAction.hh"
#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"

#include <G4Step.hh>
#include <G4Track.hh>
#include <G4StepPoint.hh>
#include <G4VProcess.hh>

namespace Test
{

SteppingAction::SteppingAction()
: G4UserSteppingAction()
{}

SteppingAction::~SteppingAction()
{}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  if (!fSipmLV) // new
  {
    const DetectorConstruction* detConstruction
      = static_cast<const DetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    
    fSipmLV = detConstruction->GetSipmLV();
  }

  // get volume of the current step
  G4LogicalVolume* volume
    = step->GetPreStepPoint()->GetTouchableHandle()
      ->GetVolume()->GetLogicalVolume();
  
  /* Debug dumping
  G4cout << "Volume: " << volume->GetName() << "\tTrack ID: " << (step->GetTrack()->GetTrackID()) << "\tParent ID: " << (step->GetTrack()->GetParentID()) << G4endl; */
  
  if (volume == fSipmLV) { // TODO: write this information into a root file - montecarlo/src/PIStepAction.cc
    G4Track* track = step->GetTrack();
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
          // G4cout << "Optical absorption in the SiPM!" << G4endl;
        }
      }
    }
  }
}

}