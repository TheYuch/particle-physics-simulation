#pragma once

#include <TObject.h>
// #include <TVector3.h>

#include "detectors/HitData.hh"

class CaloRootData : public TObject
{
private:
    Int_t caloID;

    std::vector<Float_t> edep;

public:
    CaloRootData();
    ~CaloRootData();

    virtual void Clear(Option_t* option="");
    virtual void Print(Option_t* option="") const;

    void AddData(Test::HitData* hitData);

    Int_t GetCaloID() { return caloID; };

    void SetCaloID(Int_t id) { caloID = id; };

    ClassDef(CaloRootData, 1);
};