#pragma once

#include <G4VSensitiveDetector.hh>
#include <G4HCofThisEvent.hh>

#include "detectors/DetectorHit.hh"

class G4Step;
class G4HCofThisEvent;
class G4String;
class G4TouchableHistory;

namespace Test
{

template<class HitDataT>
class SensitiveDetector : public G4VSensitiveDetector
{
public:
    SensitiveDetector() = delete;

    SensitiveDetector(G4String name)
    : G4VSensitiveDetector(name)
    , fHitsCollection(nullptr)
    {
        collectionName.insert(name + "HitsCollection");
    }

    ~SensitiveDetector()
    {}

    virtual void Initialize(G4HCofThisEvent* HCE) override
    {
        fHitsCollection = new DetectorHitsCollection(SensitiveDetectorName, collectionName[0]);

        static G4int HCID = -1;
        if (HCID < 0) HCID = GetCollectionID(0);

        HCE->AddHitsCollection(HCID, fHitsCollection);
    }

    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory*) override
    {
        if (HitDataT::IgnoreStep(step)) return false;

        DetectorHit* newHit = new DetectorHit(new HitDataT(step));
        fHitsCollection->insert(newHit);
        return true;
    }

    virtual void EndOfEvent(G4HCofThisEvent*) override
    {
        HitDataT::ConvertToRootData(fHitsCollection);
    }

private:
    DetectorHitsCollection* fHitsCollection;
};

}