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
    collectionName.insert("SipmHitsCollection");
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
    G4int detectorID = step->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber();
    G4int trackID = step->GetTrack()->GetTrackID();
    
    G4int ancestorID = fParentList[trackID];
    if (!ancestorID)
    {
        ancestorID = fParentList[step->GetTrack()->GetParentID()];
        if (!ancestorID)
        {
            ancestorID = trackID;
        }
        fParentList[trackID] = ancestorID;
    }

    DetectorHit* newHit = nullptr;
    G4int nHits = fHitsCollection->entries();
    for (G4int i = 0; i < nHits; i++)
    {
        auto h = (*fHitsCollection)[i];
        if (h->CheckIDMatch(detectorID, ancestorID)) {
            newHit = h;
            break;
        }
    }
    if (!newHit)
    {
        newHit = new DetectorHit(new SipmHitData());
        newHit->SetDetectorID(detectorID);
        newHit->SetTrackID(trackID);
        fHitsCollection->insert(newHit);
    }

    if (step->GetTrack()->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition())
    {
        newHit->UpdateData(step);

        // Once a photon hits an optical sensitive detector, kill it
        step->GetTrack()->SetTrackStatus(fKillTrackAndSecondaries);
    }
    /* else
    {
        // TODO
    } */

    return true;
}

void SipmSD::EndOfEvent(G4HCofThisEvent* HCE)
{
    std::map<int, SipmRootData*> sipmRootDataMap;

    SipmRootData* data = nullptr;
    G4int nHits = fHitsCollection->entries();
    for (G4int i = 0; i < nHits; i++)
    {
        auto h = (*fHitsCollection)[i];
        G4int detectorID = h->GetDetectorID();

        data = sipmRootDataMap[detectorID];
        if (!data)
        {
            data = RootManager::Instance()->GetNewSipmRootData();

            data->SetSipmID(detectorID);
            sipmRootDataMap[detectorID] = data;
        }

        data->AddData(h->GetData());
    }

    fParentList.clear();
}

}