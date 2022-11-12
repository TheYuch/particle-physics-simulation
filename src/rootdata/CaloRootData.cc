#include "rootdata/CaloRootData.hh"

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

void CaloRootData::Update(Float_t showerEdep)
{
    totalEdep += showerEdep;
    showerEdeps.push_back(showerEdep);
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
    for (size_t i = 0; i < showerEdeps.size(); i++)
    {
        std::cout << "\t\tEdep: " << showerEdeps.at(i) << std::endl;
    }
}