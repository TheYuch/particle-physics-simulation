#pragma once

#include <TObject.h>

class SipmData : public TObject
{
private:
    Int_t sipmID;

    Int_t nHits;
    std::vector<Float_t> times;

public:
    SipmData();
    ~SipmData();

    virtual void Clear(Option_t* option="");
    virtual void Print(Option_t* option="") const;

    void AddData(Float_t time);

    Int_t GetSipmID() {return sipmID; };

    void SetSipmID(Int_t id) {sipmID = id; };

    ClassDef(SipmData, 1);
};