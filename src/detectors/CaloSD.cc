#include <G4HCofThisEvent.hh>
#include <G4Step.hh>
#include <G4String.hh>
#include <G4TouchableHistory.hh>
#include <G4OpticalPhoton.hh>

#include "detectors/CaloSD.hh"
#include "detectors/CaloHit.hh"

#include "RootManager.hh"
#include "data/CaloData.hh"

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
    fHitsCollection = new CaloHitsCollection(SensitiveDetectorName, collectionName[0]);

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

    G4int caloID = step->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber();
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


    CaloHit* newHit = nullptr;
    G4int nHits = fHitsCollection->entries();
    for (G4int i = 0; i < nHits; i++)
    {
        auto h = (*fHitsCollection)[i];
        if (h->CheckIDMatch(caloID, ancestorID)) {
            newHit = h;
            break;
        }
    }
    if (!newHit) // existing ancestor hit information not found, must create new one
    {
        newHit = new CaloHit();
        newHit->SetCaloID(caloID);
        newHit->SetTrackID(trackID);
        fHitsCollection->insert(newHit);
    }
    
    newHit->AddData(edep);

    return true;
}

void CaloSD::EndOfEvent(G4HCofThisEvent* HCE)
{
    std::map<int, CaloData*> caloDataMap; // id <-> CaloData

    CaloData* data = nullptr;
    G4int nHits = fHitsCollection->entries();
    for (G4int i = 0; i < nHits; i++)
    {
        auto h = (*fHitsCollection)[i];

        data = caloDataMap[h->GetCaloID()];
        if (!data)
        {
            data = RootManager::Instance()->GetNewCalo();

            G4int caloID = h->GetCaloID();
            data->SetCaloID(caloID);
            caloDataMap[caloID] = data;
        }

        data->AddData(h->GetEdep());
    }

    /* // Debug dumping
    if (true) //(G4Threading::G4GetThreadId() == 1)
    {
        G4cout << "====Begin of event====" << G4endl;
        G4int cnt = 0;
        for (auto iter : fParentList)
        {
            if (iter.first == iter.second) cnt++;
            G4cout << iter.first << " " << iter.second << G4endl;
        }
        G4cout << "====End of event====" << " | cnt: " << cnt << G4endl;
    } */

    // Clean up fParentList to be ready for next event
    fParentList.clear();
}

}