#pragma once

#include <TObject.h>
// #include <TVector3.h>

#include "detectors/hitdata/HitData.hh"

class CaloRootData : public TObject
{
private:
    Int_t detectorID;

    std::vector<Float_t> edep;

public:
    CaloRootData();
    ~CaloRootData();

    virtual void Clear(Option_t* option="");
    virtual void Print(Option_t* option="") const;

    void AddData(Test::HitData* hitData);

    Int_t GetDetectorID() const { return detectorID; };
    void SetDetectorID(Int_t id) { detectorID = id; };

    ClassDef(CaloRootData, 1);
};