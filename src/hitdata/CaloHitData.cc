#include "hitdata/CaloHitData.hh"

#include <G4Step.hh>
#include <G4OpticalPhoton.hh>
#include <G4UnitsTable.hh>

#include <map>
#include "utils/RootManager.hh"
#include "rootdata/CaloRootData.hh"

namespace Test
{

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
        << "Calorimeter Geant4 Hit Data" << G4endl
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
    std::map<G4int, G4int> ancestorMap;
    std::map<G4int, CaloRootData*> detectorMap;

    G4int nHits = hitsCollection->entries();
    for (G4int i = 0; i < nHits; i++)
    {
        auto h = (*hitsCollection)[i];
        CaloHitData* caloHitData = dynamic_cast<CaloHitData*>(h->GetData());

        // ancestor matching
        G4int trackID = caloHitData->fTrackID;
        G4int ancestorID = ancestorMap[trackID];
        if (!ancestorID)
        {
            ancestorID = ancestorMap[caloHitData->fParentID];
            if (!ancestorID)
            {
                ancestorID = trackID;
            }
            ancestorMap[trackID] = ancestorID;
        }

        // detector matching - TODO: do ancestor mapping here to reduce rootdata memory below?
        G4int detectorID = caloHitData->fDetectorID;
        CaloRootData* data = detectorMap[detectorID];
        if (!data)
        {
            data = (CaloRootData*)RootManager::Instance()->GetNewRootData(Constants::RootDataType::Calo);
            data->Initialize(detectorID);
            detectorMap[detectorID] = data;
        }
        data->Update(caloHitData, ancestorID);
    }
}

}