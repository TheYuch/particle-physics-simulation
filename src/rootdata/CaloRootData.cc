#include "rootdata/CaloRootData.hh"
#include <iostream>

ClassImp(CaloRootData)

CaloRootData::CaloRootData()
: TObject()
, caloID(0)
{}

CaloRootData::~CaloRootData()
{}

void CaloRootData::Clear(Option_t* option)
{
    edep.clear();
}

void CaloRootData::Print(Option_t* option) const
{
    std::cout << "Calo: " << caloID << std::endl;
    for (size_t i = 0; i < edep.size(); i++) {
        std::cout << " > edep: " << edep.at(i) << std::endl;
    }
}

void CaloRootData::AddData(Float_t energy)
{
    edep.push_back(energy);
}