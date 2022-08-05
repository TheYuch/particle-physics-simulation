#pragma once

#include <TObject.h>

namespace Test
{
    class HitData;
}

class SipmRootData : public TObject
{
private:
    Int_t detectorID;

    Int_t nHits;
    std::vector<Float_t> times;

public:
    SipmRootData();
    ~SipmRootData();

    virtual void Clear(Option_t* option="") override;
    virtual void Print(Option_t* option="") const override;

    void AddData(Test::HitData* hitData);

    Int_t GetDetectorID() const { return detectorID; };
    void SetDetectorID(Int_t id) { detectorID = id; };

    ClassDef(SipmRootData, 1);
};