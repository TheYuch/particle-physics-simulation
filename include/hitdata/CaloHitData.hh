#pragma once

#include "hitdata/HitData.hh"

#include "detectors/DetectorHit.hh"

class G4Step;

class CaloHitData : public HitData
{
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