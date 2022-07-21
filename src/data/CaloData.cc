#include "data/CaloData.hh"
#include <iostream>

ClassImp(CaloData)

CaloData::CaloData()
: TObject()
, caloID(0)
{}

CaloData::~CaloData()
{}

void CaloData::Clear(Option_t* option)
{
    edep.clear();
}

void CaloData::Print(Option_t* option) const
{
    for (size_t i = 0; i < edep.size(); i++) {
        std::cout << " > edep: " << edep.at(i) << std::endl;
    }
}

void CaloData::AddData(Float_t energy)
{
    edep.push_back(energy);
}