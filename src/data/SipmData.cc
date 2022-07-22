#include "data/SipmData.hh"
#include <iostream>

ClassImp(SipmData)

SipmData::SipmData()
: TObject()
, sipmID(0)
, nHits(0)
{}

SipmData::~SipmData()
{}

void SipmData::Clear(Option_t* option)
{
    nHits = 0;
    times.clear();
}

void SipmData::Print(Option_t* option) const
{
    std::cout << "Sipm: " << sipmID << ", number of hits: " << nHits << std::endl;
    for (size_t i = 0; i < times.size(); i++) {
        std::cout << " > time: " << times.at(i) << std::endl;
    }
}

void SipmData::AddData(Float_t time)
{
    nHits++;
    times.push_back(time);
}