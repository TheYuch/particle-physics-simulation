#pragma once

#include "detectors/SipmHit.hh"
#include <G4VSensitiveDetector.hh>

#include <map>

class G4Step;
class G4HCofThisEvent;
class G4String;
class G4TouchableHistory;

namespace Test
{

class SipmSD : public G4VSensitiveDetector
{
public:
    SipmSD(G4String name);
    ~SipmSD();

    virtual void Initialize(G4HCofThisEvent* HCE) override;
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;
    virtual void EndOfEvent(G4HCofThisEvent* HCE) override;

private:
    SipmHitsCollection* fHitsCollection;

    std::map<int, int> fParentList;
};

}