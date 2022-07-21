#pragma once

#include "G4VSensitiveDetector.hh"
#include "detectors/CaloHit.hh"

#include <map>

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

    virtual void Initialize(G4HCofThisEvent* HCE) override;
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;
    virtual void EndOfEvent(G4HCofThisEvent* HCE) override;

private:
    CaloHitsCollection* fHitsCollection;

    std::map<int, int> fParentList;
};

}