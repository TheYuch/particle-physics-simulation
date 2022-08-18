#pragma once

#include <TObject.h>

#include <map>
// #include <TVector3.h>

class CaloRootData : public TObject
{
private:
    Int_t detectorID;

    Float_t totalEdep;
    std::map<Int_t, Float_t> showerEdeps;

public:
    CaloRootData();
    ~CaloRootData();

    void Initialize(Int_t id);
    void Update(Float_t edep, Int_t ancestorID);

    virtual void Clear(Option_t* option="") override;
    virtual void Print(Option_t* option="") const override;

    // TODO: get functions for all data attributes for post-Geant4 processing

    ClassDef(CaloRootData, 1);
};