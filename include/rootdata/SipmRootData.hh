#pragma once

#include <TObject.h>

#include "detectors/HitData.hh"

class SipmRootData : public TObject
{
private:
    Int_t sipmID;

    Int_t nHits;
    std::vector<Float_t> times;

public:
    SipmRootData();
    ~SipmRootData();

    virtual void Clear(Option_t* option="");
    virtual void Print(Option_t* option="") const;

    void AddData(Test::HitData* hitData);

    Int_t GetSipmID() { return sipmID; };

    void SetSipmID(Int_t id) { sipmID = id; };

    ClassDef(SipmRootData, 1);
};