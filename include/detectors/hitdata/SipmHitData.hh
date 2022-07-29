#include "detectors/HitData.hh"
#include <G4UnitsTable.hh>
#include <G4Step.hh>

class SipmRootData;

namespace Test
{

class SipmHitData : public HitData
{
friend class ::SipmRootData;
public:
    SipmHitData()
    : fTime(0)
    {}

    ~SipmHitData()
    {}

    const HitData& operator=(const HitData& other)
    {
        const SipmHitData* otherData = dynamic_cast<const SipmHitData*>(&other);
        fTime = otherData->fTime;
        
        return *this;
    }

    void Update(G4Step* step)
    {
        G4double time = step->GetPostStepPoint()->GetGlobalTime();
        fTime = time;
    }

    void Print(G4int detectorID, G4int trackID)
    {
        G4cout
            << "SiPM..." << G4endl
            << "    Detector ID: " << detectorID << G4endl
            << "    Track ID: " << trackID << G4endl
            << "    Time: " << G4BestUnit(fTime, "Time")
            << G4endl;
    }

private:
    G4double fTime;
};

}