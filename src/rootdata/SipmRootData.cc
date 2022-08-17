#include "rootdata/SipmRootData.hh"

#include "detectors/hitdata/SipmHitData.hh"

#include <iostream>

ClassImp(SipmRootData)

SipmRootData::SipmRootData()
: TObject()
, detectorID(0)
, nHits(0)
, totalEdep(0)
{}

SipmRootData::~SipmRootData()
{}

void SipmRootData::Initialize(Int_t id)
{
    detectorID = id;
}

void SipmRootData::Update(Test::SipmHitData* sipmHitData)
{
    nHits ++;
    times.push_back(sipmHitData->fTime);

    totalEdep += sipmHitData->fEdep;
}

void SipmRootData::Clear(Option_t* option)
{
    nHits = 0;
    times.clear();
    totalEdep = 0;
}

void SipmRootData::Print(Option_t* option) const
{
    std::cout
        << "Sipm Root Data:" << std::endl
        << "\tDetector ID: " << detectorID << std::endl
        << "\tNumber of hits: " << nHits << std::endl
        << "\tTimes of hits:" << std::endl;
    for (size_t i = 0; i < times.size(); i++)
    {
        std::cout << "\t\tTime: " << times.at(i) << std::endl;
    }
    std::cout
        << "\tTotal Energy Deposit of particles that are not optical photons: " << totalEdep << std::endl;
}