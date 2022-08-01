#include "rootdata/SipmRootData.hh"
#include <iostream>

#include "detectors/hitdata/HitData.hh"
#include "detectors/hitdata/SipmHitData.hh"

ClassImp(SipmRootData)

SipmRootData::SipmRootData()
: TObject()
, detectorID(0)
, nHits(0)
{}

SipmRootData::~SipmRootData()
{}

void SipmRootData::Clear(Option_t* option)
{
    nHits = 0;
    times.clear();
}

void SipmRootData::Print(Option_t* option) const
{
    std::cout << "Sipm: " << detectorID << ", number of hits: " << nHits << std::endl;
    for (size_t i = 0; i < times.size(); i++) {
        std::cout << " > time: " << times.at(i) << std::endl;
    }
}

void SipmRootData::AddData(Test::HitData* hitData)
{
    Test::SipmHitData* sipmHitData = dynamic_cast<Test::SipmHitData*>(hitData);
    nHits++;
    times.push_back(sipmHitData->fTime);
}