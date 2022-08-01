#pragma once

#include <TObject.h>

#include "detectors/hitdata/HitData.hh"

class RootData : public TObject
{
private:
    Int_t detectorID; // no 'f' in front because this member is public for root

public:
    RootData();
    ~RootData();

    virtual void Clear(Option_t* option="");
    virtual void Print(Option_t* option="") const;

    virtual void AddData(Test::HitData* hitData) = 0;

    Int_t GetDetectorID() const { return detectorID; };
    void SetDetectorID(Int_t id) { detectorID = id; };
};