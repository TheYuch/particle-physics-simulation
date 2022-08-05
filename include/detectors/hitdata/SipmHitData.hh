#include "detectors/hitdata/HitData.hh"
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
    {}

    ~SipmHitData()
    {}

    const HitData& operator=(const HitData& other)
    {
        const SipmHitData* otherData = dynamic_cast<const SipmHitData*>(&other);
        fTimes = otherData->fTimes;
        
        return *this;
    }

    void Update(G4Step* step)
    {
        G4double time = step->GetPostStepPoint()->GetGlobalTime();
        fTimes.push_back(time);
    }

    void Print(G4int detectorID, G4int trackID)
    {
        G4cout
            << "SiPM..." << G4endl
            << "    Detector ID: " << detectorID << G4endl
            << "    Track ID: " << trackID << G4endl
            << "    Number of optical hits: " << fTimes.size()
            << G4endl;
    }

private:
    std::vector<G4double> fTimes;
};

}