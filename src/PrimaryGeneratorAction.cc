#include "PrimaryGeneratorAction.hh"

#include <G4LogicalVolumeStore.hh>
#include <G4LogicalVolume.hh>
#include <G4Box.hh>
#include <G4RunManager.hh>
#include <G4ParticleGun.hh>
#include <G4Positron.hh>
#include <G4ParticleDefinition.hh>
#include <G4SystemOfUnits.hh>

#include "Randomize.hh"

namespace Test
{

PrimaryGeneratorAction::PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction()
{
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  fParticleGun->SetParticleDefinition(G4Positron::PositronDefinition());
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  fParticleGun->SetParticleEnergy(69.*MeV);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  G4double worldZHalfLength = 0;
  G4LogicalVolume* worldLV = G4LogicalVolumeStore::GetInstance()->GetVolume("World");
  G4Box* worldS = nullptr;
  if ( worldLV ) worldS = dynamic_cast<G4Box*>(worldLV->GetSolid());
  if ( worldS ) worldZHalfLength = worldS->GetZHalfLength();
  else  {
    G4cerr << "World volume of box not found." << G4endl;
    G4cerr << "Perhaps you have changed geometry." << G4endl;
    G4cerr << "The gun will be place in the center." << G4endl;
  }

  fParticleGun->SetParticlePosition(G4ThreeVector(
    (G4UniformRand() - 0.5) * 8*cm,
    (G4UniformRand() - 0.5) * 8*cm,
    -worldZHalfLength));

  fParticleGun->GeneratePrimaryVertex(anEvent);
}

}