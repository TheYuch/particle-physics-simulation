#pragma once

#include <G4Step.hh>
#include <G4Types.hh>

namespace Test
{

class HitData
{
public:
    HitData() = delete;
    HitData(G4Step* step)
    {
        fDetectorID = step->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber();
    }
    virtual ~HitData() = default;

    virtual void Print() = 0;

protected:
    G4int fDetectorID;
};

}