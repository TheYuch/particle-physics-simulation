#include "hitdata/SipmHitData.hh"

#include <G4Step.hh>
#include <G4OpticalPhoton.hh>
#include <G4UnitsTable.hh>

#include <map>
#include "utils/RootManager.hh"
#include "rootdata/SipmRootData.hh"

namespace Test
{

SipmHitData::SipmHitData(G4Step* step)
: HitData(step)
{
    fParticleDefinition = step->GetTrack()->GetDefinition();
    if (fParticleDefinition == G4OpticalPhoton::OpticalPhotonDefinition())
    {
        fTime = step->GetPostStepPoint()->GetGlobalTime();
        fEdep = 0;
        
        step->GetTrack()->SetTrackStatus(G4TrackStatus::fKillTrackAndSecondaries);
    }
    else
    {
        fEdep = step->GetTotalEnergyDeposit();
        fTime = 0;
    }
    // TODO: time only stored for optical photons, and edep only stored for edep - unless not???? would this if statement be more arbitrary/confusing??
}

SipmHitData::~SipmHitData() // fParticleDefinition not owned here, so no need to delete
{}

void SipmHitData::Print()
{
    G4cout
        << "Sipm Geant4 Hit Data:" << G4endl
        << "\tDetector ID: " << fDetectorID << G4endl
        << "\tParticle Name: " << fParticleDefinition->GetParticleName() << G4endl
        << "\tOptical Hit Time:" << G4BestUnit(fTime, "Time") << G4endl
        << "\tNon-Optical Energy Deposit: " << G4BestUnit(fEdep, "Energy") << G4endl;
}

G4bool SipmHitData::IgnoreStep(G4Step* step) // Note: steps with edep <= 0 are still kept to allow ancestor tracking to work
{
    if (step->GetTrack()->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition())
    {
        if (step->GetTotalEnergyDeposit() <= 0)
        {
            return true;
        }
    }
    return false;
}

void SipmHitData::ConvertToRootData(DetectorHitsCollection* hitsCollection)
{
    std::map<G4int, SipmRootData*> detectorMap;

    G4int nHits = hitsCollection->entries();
    for (G4int i = 0; i < nHits; i++)
    {
        auto h = (*hitsCollection)[i];
        SipmHitData* sipmHitData = dynamic_cast<SipmHitData*>(h->GetData());

        G4int detectorID = sipmHitData->fDetectorID;
        SipmRootData* data = detectorMap[detectorID];
        if (!data)
        {
            data = (SipmRootData*)RootManager::Instance()->GetNewRootData(Constants::RootDataType::Sipm);
            data->Initialize(detectorID);
            detectorMap[detectorID] = data;
        }
        if (sipmHitData->fParticleDefinition == G4OpticalPhoton::OpticalPhotonDefinition())
            data->UpdateOptical(sipmHitData->fTime);
        else
            data->UpdateConventional(sipmHitData->fEdep);
    }
}

}