#pragma once

#include "detectors/HitData.hh"
#include "globals.hh"

namespace Test
{

struct CaloHitData : HitData
{
public:
    CaloHitData();
    ~CaloHitData() override;

    const HitData& operator=(const HitData& other) override;
    void Update(HitData* data) override;
    void Print() override;

private:
    G4double fEdep;
}

}