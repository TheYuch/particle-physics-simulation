#include "hitdata/CaloHitData.hh"

#include <G4Step.hh>
#include <G4OpticalPhoton.hh>
#include <G4UnitsTable.hh>

#include <map>
#include "utils/RootManager.hh"
#include "rootdata/CaloRootData.hh"

CaloHitData::CaloHitData(G4Step* step)
: HitData(step)
{
    fTrackID = step->GetTrack()->GetTrackID();
    fParentID = step->GetTrack()->GetParentID();
    fEdep = step->GetTotalEnergyDeposit();
}

CaloHitData::~CaloHitData()
{}

void CaloHitData::Print()
{
    G4cout
        << "Calorimeter Geant4 Hit Data:" << G4endl
        << "\tDetector ID: " << fDetectorID << G4endl
        << "\tTrack ID: " << fTrackID << G4endl
        << "\tParent ID: " << fParentID << G4endl
        << "\tEnergy Deposit: " << G4BestUnit(fEdep, "Energy") << G4endl;
}

G4bool CaloHitData::IgnoreStep(G4Step* step) // Note: steps with edep <= 0 are still kept to allow ancestor tracking to work
{
    if (step->GetTrack()->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition())
    {
        // Don't care about optical photons here. They will be detected by SiPMs
        return true;
    }
    return false;
}

void CaloHitData::ConvertToRootData(DetectorHitsCollection* hitsCollection)
{
    std::map<G4int, G4int> globalAncestorMap;
    std::map<G4int, std::map<G4int, std::vector<CaloHitData*>>> detectorMap;

    G4int nHits = hitsCollection->entries();
    for (G4int i = 0; i < nHits; i++)
    {
        auto h = (*hitsCollection)[i];
        CaloHitData* caloHitData = dynamic_cast<CaloHitData*>(h->GetData());

        // get shower ancestor ID
        G4int trackID = caloHitData->fTrackID;
        G4int ancestorID = globalAncestorMap[trackID];
        if (!ancestorID)
        {
            ancestorID = globalAncestorMap[caloHitData->fParentID];
            if (!ancestorID)
            {
                ancestorID = trackID;
            }
            globalAncestorMap[trackID] = ancestorID;
        }

        // match detector and ancestor and add data
        G4int detectorID = caloHitData->fDetectorID;
        detectorMap[detectorID][ancestorID].push_back(caloHitData);
    }

    for (auto const& detectorPair : detectorMap)
    {
        CaloRootData* data = (CaloRootData*)RootManager::Instance()->GetNewRootData(Constants::RootDataType::Calo);
        data->Initialize(detectorPair.first);
        for (auto const& showerPair : detectorPair.second)
        {
            G4double showerEdep = 0;
            for (auto& caloHitData : showerPair.second)
            {
                showerEdep += caloHitData->fEdep;
            }
            
            data->Update(showerEdep);
        }
    }
}