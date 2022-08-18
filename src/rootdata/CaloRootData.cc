#include "rootdata/CaloRootData.hh"

#include "hitdata/CaloHitData.hh"

#include <iostream>

ClassImp(CaloRootData)

CaloRootData::CaloRootData()
: TObject()
, detectorID(0)
{}

CaloRootData::~CaloRootData()
{}

void CaloRootData::Initialize(Int_t id)
{
    detectorID = id;
}

void CaloRootData::Update(Test::CaloHitData* caloHitData, Int_t ancestorID)
{
    totalEdep += caloHitData->fEdep;
    showerEdeps[ancestorID] += caloHitData->fEdep;
}

void CaloRootData::Clear(Option_t* option)
{
    totalEdep = 0;
    showerEdeps.clear();
}

void CaloRootData::Print(Option_t* option) const
{
    std::cout
        << "Calorimeter Root Data:" << std::endl
        << "\tDetector ID: " << detectorID << std::endl
        << "\tTotal Edep: " << totalEdep << std::endl
        << "\tShower Edeps:" << std::endl;
    for (auto const& showerEdepPair : showerEdeps)
    {
        std::cout << "\t\tAncestor Track ID: " << showerEdepPair.first << ", Edep: " << showerEdepPair.second << std::endl;
    }
}