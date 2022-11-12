#pragma once

#include <TObject.h>

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
    void UpdateConventional(Float_t edep);
    void UpdateOptical(Float_t time);

    virtual void Clear(Option_t* option="") override;
    virtual void Print(Option_t* option="") const override;

    ClassDef(SipmRootData, 1);
};