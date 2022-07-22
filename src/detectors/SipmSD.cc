#include <G4HCofThisEvent.hh>
#include <G4Step.hh>
#include <G4String.hh>
#include <G4TouchableHistory.hh>
#include <G4OpticalPhoton.hh>
#include <G4SDManager.hh>

#include "detectors/SipmSD.hh"
#include "detectors/SipmHit.hh"

#include "RootManager.hh"
#include "data/SipmData.hh"

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
    fHitsCollection = new SipmHitsCollection(SensitiveDetectorName, collectionName[0]);

    static G4int HCID = -1;
    if (HCID < 0) HCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection);

    HCE->AddHitsCollection(HCID, fHitsCollection);
}

G4bool SipmSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
    G4int sipmID = step->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber();
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

    SipmHit* newHit = nullptr;
    G4int nHits = fHitsCollection->entries();
    for (G4int i = 0; i < nHits; i++)
    {
        auto h = (*fHitsCollection)[i];
        if (h->CheckIDMatch(sipmID, ancestorID)) {
            newHit = h;
            break;
        }
    }
    if (!newHit)
    {
        newHit = new SipmHit();
        newHit->SetSipmID(sipmID);
        newHit->SetTrackID(trackID);
        fHitsCollection->insert(newHit);
    }

    if (step->GetTrack()->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition())
    {
        G4double time = step->GetPostStepPoint()->GetGlobalTime();
        newHit->AddData(time);

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
    std::map<int, SipmData*> sipmDataMap;

    SipmData* data = nullptr;
    G4int nHits = fHitsCollection->entries();
    for (G4int i = 0; i < nHits; i++)
    {
        auto h = (*fHitsCollection)[i];

        data = sipmDataMap[h->GetSipmID()];
        if (!data)
        {
            data = RootManager::Instance()->GetNewSipm();

            G4int sipmID = h->GetSipmID();
            data->SetSipmID(sipmID);
            sipmDataMap[sipmID] = data;
        }

        data->AddData(h->GetTime());
    }

    fParentList.clear();
}

}