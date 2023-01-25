#pragma once

#include "hitdata/HitData.hh"

#include "detectors/DetectorHit.hh"

class G4Step;

class SipmHitData : public HitData
{
public:
    SipmHitData() = delete;
    SipmHitData(G4Step* step);
    ~SipmHitData();

    virtual void Print() override;

    static G4bool IgnoreStep(G4Step* step);
    static void ConvertToRootData(DetectorHitsCollection* hitsCollection);

private:
    G4ParticleDefinition* fParticleDefinition;
    G4double fTime;
    G4double fEdep;
};