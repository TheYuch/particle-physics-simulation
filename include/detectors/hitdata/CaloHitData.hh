#pragma once

#include "detectors/hitdata/HitData.hh"

#include "detectors/DetectorHit.hh"

class G4Step;

class CaloRootData;

namespace Test
{

class CaloHitData : public HitData
{
friend class ::CaloRootData;
public:
    CaloHitData() = delete;
    CaloHitData(G4Step* step);
    ~CaloHitData();

    virtual void Print() override;

    static G4bool IgnoreStep(G4Step* step);
    static void ConvertToRootData(DetectorHitsCollection* hitsCollection);

private:
    G4int fTrackID;
    G4int fParentID;
    G4double fEdep;
};

}