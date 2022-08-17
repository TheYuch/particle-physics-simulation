#include "detectors/CaloSD.hh"

#include <G4HCofThisEvent.hh>

#include "detectors/DetectorHit.hh"
#include "detectors/hitdata/CaloHitData.hh"

class CaloRootData;

namespace Test
{

CaloSD::CaloSD(G4String name)
: G4VSensitiveDetector(name)
, fHitsCollection(nullptr)
{
    collectionName.insert(name + "HitsCollection");
}

CaloSD::~CaloSD()
{}

void CaloSD::Initialize(G4HCofThisEvent* HCE)
{
    fHitsCollection = new DetectorHitsCollection(SensitiveDetectorName, collectionName[0]);

    static G4int HCID = -1;
    if (HCID < 0) HCID = GetCollectionID(0);

    HCE->AddHitsCollection(HCID, fHitsCollection);
}

G4bool CaloSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
    if (CaloHitData::IgnoreStep(step)) return false;

    DetectorHit* newHit = new DetectorHit(new CaloHitData(step));
    fHitsCollection->insert(newHit);
    return true;
}

void CaloSD::EndOfEvent(G4HCofThisEvent*)
{
    CaloHitData::ConvertToRootData(fHitsCollection);
}

}