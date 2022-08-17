#pragma once

#include <G4VSensitiveDetector.hh>

#include "detectors/DetectorHit.hh"

class G4Step;
class G4HCofThisEvent;
class G4String;
class G4TouchableHistory;

namespace Test
{

class CaloSD : public G4VSensitiveDetector
{
public:
    CaloSD(G4String name);
    ~CaloSD();

    virtual void Initialize(G4HCofThisEvent*) override;
    virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*) override;
    virtual void EndOfEvent(G4HCofThisEvent*) override;

private:
    DetectorHitsCollection* fHitsCollection;
};

}