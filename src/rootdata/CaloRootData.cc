#include "rootdata/CaloRootData.hh"
#include <iostream>

#include "detectors/hitdata/CaloHitData.hh"

ClassImp(CaloRootData)

CaloRootData::CaloRootData()
: TObject()
, detectorID(0)
{}

CaloRootData::~CaloRootData()
{}

void CaloRootData::Clear(Option_t* option)
{
    edep.clear();
}

void CaloRootData::Print(Option_t* option) const
{
    std::cout << "Calo: " << detectorID << std::endl;
    for (size_t i = 0; i < edep.size(); i++) {
        std::cout << " > edep: " << edep.at(i) << std::endl;
    }
}

void CaloRootData::AddData(Test::HitData* hitData)
{
    Test::CaloHitData* caloHitData = dynamic_cast<Test::CaloHitData*>(hitData);
    edep.push_back(caloHitData->fEdep);
}