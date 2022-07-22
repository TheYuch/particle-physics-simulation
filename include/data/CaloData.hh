#pragma once

#include <TObject.h>
// #include <TVector3.h>

class CaloData : public TObject
{
private:
    Int_t caloID;

    std::vector<Float_t> edep;

public:
    CaloData();
    ~CaloData();

    virtual void Clear(Option_t* option="");
    virtual void Print(Option_t* option="") const;

    void AddData(Float_t energy);

    Int_t GetCaloID() {return caloID; };

    void SetCaloID(Int_t id) {caloID = id; };

    ClassDef(CaloData, 1);
};