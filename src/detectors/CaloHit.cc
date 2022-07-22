#include "detectors/CaloHit.hh"
#include "G4UnitsTable.hh"

namespace Test
{

G4ThreadLocal G4Allocator<CaloHit>* CaloHitAllocator = nullptr;

CaloHit::CaloHit()
: G4VHit()
, fCaloID(-1)
, fTrackID(-1)
, fEdep(0)
{}

CaloHit::CaloHit(const CaloHit& other)
: G4VHit()
, fCaloID(other.fCaloID)
, fTrackID(other.fTrackID)
, fEdep(other.fEdep)
{}

CaloHit::~CaloHit()
{}

const CaloHit& CaloHit::operator=(const CaloHit& other)
{
    fCaloID = other.fCaloID;
    fTrackID = other.fTrackID;
    fEdep = other.fEdep;

    return *this;
}

G4bool CaloHit::CheckIDMatch(G4int caloID, G4int trackID)
{
    return (caloID == fCaloID) && (trackID == fTrackID);
}

void CaloHit::AddData(G4double edep)
{
    fEdep += edep;

    // TODO: record something else with individual edeps, such as edep/distance (dE/dx) {opt: vs time}?
}

void CaloHit::Print()
{
    G4cout
        << "Calo ID: " << fCaloID << G4endl
        << "    TrackID: " << fTrackID << G4endl
        << "    Edep: " << G4BestUnit(fEdep, "Energy")
        << G4endl;
}

}