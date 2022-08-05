#include <G4HCofThisEvent.hh>
#include <G4Step.hh>
#include <G4String.hh>
#include <G4TouchableHistory.hh>
#include <G4OpticalPhoton.hh>

#include "detectors/SipmSD.hh"
#include "detectors/DetectorHit.hh"
#include "detectors/hitdata/SipmHitData.hh"

#include "RootManager.hh"
#include "rootdata/SipmRootData.hh"

namespace Test
{

SipmSD::SipmSD(G4String name)
: G4VSensitiveDetector(name)
, fHitsCollection(nullptr)
{
    collectionName.insert(name + "HitsCollection");
}

SipmSD::~SipmSD() // note: fDetectorMap pointers do not have to be deleted as they are owned by fHitsCollection, which is deconstructed elsewhere
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
    if (step->GetTrack()->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition())
    {
        // initialize new hit
        G4int detectorID = step->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber();

        DetectorHit* newHit = fDetectorMap[detectorID];

        if (!newHit)
        {
            newHit = new DetectorHit(new SipmHitData());
            newHit->SetDetectorID(detectorID);

            fHitsCollection->insert(newHit);
            fDetectorMap[detectorID] = newHit;
        }
        
        newHit->UpdateData(step);

        // kill the photon that hits
        step->GetTrack()->SetTrackStatus(fKillTrackAndSecondaries);

        return true;
    }
    
    // TODO: what if the track is not an optical photon?
    
    return false;
}

void SipmSD::EndOfEvent(G4HCofThisEvent* HCE)
{
    G4int nHits = fHitsCollection->entries();
    for (G4int i = 0; i < nHits; i++)
    {
        auto h = (*fHitsCollection)[i];

        SipmRootData* data = (SipmRootData*)RootManager::Instance()->GetNewRootData(Constants::RootDataType::Sipm);
        data->SetDetectorID(h->GetDetectorID());
        data->AddData(h->GetData());
    }

    fDetectorMap.clear();
}

}