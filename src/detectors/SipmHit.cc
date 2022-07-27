#include "detectors/SipmHit.hh"
#include <G4UnitsTable.hh>

namespace Test
{

G4ThreadLocal G4Allocator<SipmHit> *SipmHitAllocator = nullptr;

SipmHit::SipmHit()
: G4VHit()
, fSipmID(-1)
, fTrackID(-1)
, fTime(-1)
{}

SipmHit::~SipmHit()
{}

const SipmHit& SipmHit::operator=(const SipmHit& other)
{
    fSipmID = other.fSipmID;
    fTrackID = other.fTrackID;
    fTime = other.fTime;

    return *this;
}

G4bool SipmHit::CheckIDMatch(G4int sipmID, G4int trackID)
{
    return (sipmID == fSipmID) && (trackID == fTrackID);
}

void SipmHit::AddData(G4double time)
{
    fTime = time;
}

void SipmHit::Print()
{
    G4cout
        << "Sipm ID: " << fSipmID << G4endl
        << "    TrackID: " << fTrackID << G4endl
        << "    Time: " << G4BestUnit(fTime, "Time")
        << G4endl;
}

}