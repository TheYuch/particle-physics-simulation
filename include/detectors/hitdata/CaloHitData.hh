#include "detectors/HitData.hh"
#include <G4UnitsTable.hh>
#include <G4Step.hh>

class CaloRootData;

namespace Test
{

class CaloHitData : public HitData
{
friend class ::CaloRootData; // TODO: tmp - declare an abstract RootData class as a friend in the base HitData class
public:
    CaloHitData() // note: if there is a relevant base class constructor with parameters, then call it here
    : fEdep(0)
    {}

    ~CaloHitData()
    {}

    const HitData& operator=(const HitData& other)
    {
        const CaloHitData* otherData = dynamic_cast<const CaloHitData*>(&other);
        fEdep = otherData->fEdep;
        
        return *this;
    }

    void Update(G4Step* step)
    {
        G4double edep = step->GetTotalEnergyDeposit();
        fEdep += edep;
    }

    void Print(G4int detectorID, G4int trackID)
    {
        G4cout
            << "CALORIMETER..." << G4endl
            << "    Detector ID: " << detectorID << G4endl
            << "    Track ID: " << trackID << G4endl
            << "    Energy Deposit: " << G4BestUnit(fEdep, "Energy")
            << G4endl;
    }

private:
    G4double fEdep;
};

}