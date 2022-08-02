#include <G4HCofThisEvent.hh>
#include <G4Step.hh>
#include <G4String.hh>
#include <G4TouchableHistory.hh>
#include <G4OpticalPhoton.hh>

#include "detectors/CaloSD.hh"
#include "detectors/DetectorHit.hh"
#include "detectors/hitdata/CaloHitData.hh"

#include "Constants.hh"
#include "RootManager.hh"
#include "rootdata/CaloRootData.hh"

namespace Test
{

CaloSD::CaloSD(G4String name)
: G4VSensitiveDetector(name)
, fHitsCollection(nullptr)
{
    collectionName.insert("CaloHitsCollection");
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
    if (step->GetTrack()->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()) {
        // Don't care about optical photons here. They will be detected by SiPMs
        return false;
    }

    G4int detectorID = step->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber();
    G4int trackID = step->GetTrack()->GetTrackID();
    
    // Get ancestor ID and/or update fParentList
    G4int ancestorID = fParentList[trackID];
    if (!ancestorID)
    {
        ancestorID = fParentList[step->GetTrack()->GetParentID()]; // trace back up the tree, valid with the Geant4 branching execution order
        if (!ancestorID) // the ancestor does not have an ancestor
        {
            ancestorID = trackID;
        }
        fParentList[trackID] = ancestorID;
    }

    G4double edep = step->GetTotalEnergyDeposit();
    if (edep <= 0.) return false; // want interesting steps with positive energy deposit.
    // However, the above must be called after fParentList manipulation because a step without energy deposit can still branch off into child particles.


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
    if (!newHit) // existing ancestor hit information not found, must create new one
    {
        newHit = new DetectorHit(new CaloHitData());
        newHit->SetDetectorID(detectorID);
        newHit->SetTrackID(trackID);
        fHitsCollection->insert(newHit);
    }
    
    newHit->UpdateData(step);

    return true;
}

void CaloSD::EndOfEvent(G4HCofThisEvent* HCE)
{
    std::map<int, CaloRootData*> caloRootDataMap;

    CaloRootData* data = nullptr;
    G4int nHits = fHitsCollection->entries();
    for (G4int i = 0; i < nHits; i++)
    {
        auto h = (*fHitsCollection)[i];
        G4int detectorID = h->GetDetectorID();

        data = caloRootDataMap[detectorID];
        if (!data)
        {
            data = (CaloRootData*)RootManager::Instance()->GetNewRootData(Constants::RootDataTypes::Calo);

            data->SetDetectorID(detectorID);
            caloRootDataMap[detectorID] = data;
        }

        data->AddData(h->GetData());
    }

    // Clean up fParentList to be ready for next event
    fParentList.clear();
}

}