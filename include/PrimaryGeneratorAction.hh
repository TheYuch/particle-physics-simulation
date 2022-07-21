#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"

class G4ParticleGun;
class G4Event;
class G4Box;

namespace Test
{

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction();
    ~PrimaryGeneratorAction();

    void GeneratePrimaries(G4Event*) override;
    
    const G4ParticleGun* GetParticleGun() const { return fParticleGun; }

  private:
    G4ParticleGun* fParticleGun = nullptr; // pointer a to G4 gun class
};

}