#pragma once

#include <TObject.h>

namespace Test
{
    class SipmHitData;
}

class SipmRootData : public TObject
{
private:
    Int_t detectorID;

    Int_t nHits;
    std::vector<Float_t> times;
    
    Float_t totalEdep; // total energy deposit of particles that are not optical photons

public:
    SipmRootData();
    ~SipmRootData();

    void Initialize(Int_t id);
    void Update(Test::SipmHitData* sipmHitData);

    virtual void Clear(Option_t* option="") override;
    virtual void Print(Option_t* option="") const override;    

    ClassDef(SipmRootData, 1);
};