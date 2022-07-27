#include "rootdata/SipmRootData.hh"
#include <iostream>

ClassImp(SipmRootData)

SipmRootData::SipmRootData()
: TObject()
, sipmID(0)
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
    std::cout << "Sipm: " << sipmID << ", number of hits: " << nHits << std::endl;
    for (size_t i = 0; i < times.size(); i++) {
        std::cout << " > time: " << times.at(i) << std::endl;
    }
}

void SipmRootData::AddData(Float_t time)
{
    nHits++;
    times.push_back(time);
}