#include "detectors/SipmSD.hh"

#include <G4HCofThisEvent.hh>

#include "detectors/DetectorHit.hh"
#include "detectors/hitdata/SipmHitData.hh"

class SipmRootData;

namespace Test
{

SipmSD::SipmSD(G4String name)
: G4VSensitiveDetector(name)
, fHitsCollection(nullptr)
{
    collectionName.insert(name + "HitsCollection");
}

SipmSD::~SipmSD()
{}

void SipmSD::Initialize(G4HCofThisEvent* HCE)
{
    fHitsCollection = new DetectorHitsCollection(SensitiveDetectorName, collectionName[0]);

    static G4int HCID = -1;
    if (HCID < 0) HCID = GetCollectionID(0);

    HCE->AddHitsCollection(HCID, fHitsCollection);
}

G4bool SipmSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
    if (SipmHitData::IgnoreStep(step)) return false;
        
    DetectorHit* newHit = new DetectorHit(new SipmHitData(step));
    fHitsCollection->insert(newHit);
    return true;
}

void SipmSD::EndOfEvent(G4HCofThisEvent*)
{
    SipmHitData::ConvertToRootData(fHitsCollection);
}

}