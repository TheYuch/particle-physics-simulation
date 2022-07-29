#pragma once

#include <G4Types.hh>

class G4Step;

namespace Test
{

class HitData
{
public:
    HitData() {}
    virtual ~HitData() = default;

    virtual const HitData& operator=(const HitData& other) = 0;
    virtual void Update(G4Step* step) = 0;
    virtual void Print(G4int detectorID, G4int trackID) = 0;
};

}