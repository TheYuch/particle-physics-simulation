#include "detectors/DetectorHit.hh"
#include <G4UnitsTable.hh>

namespace Test
{

G4ThreadLocal G4Allocator<DetectorHit>* DetectorHitAllocator = nullptr;

DetectorHit::DetectorHit()
: G4VHit()
, fDetectorID(-1)
, fTrackID(-1)
, fData()
{}

DetectorHit::DetectorHit(const DetectorHit& other)
: G4VHit()
, fDetectorID(other.fDetectorID)
, fTrackID(other.fTrackID)
, fData(other.fData)
{}

DetectorHit::~DetectorHit()
{
    delete fData;
}

const DetectorHit& DetectorHit::operator=(const DetectorHit& other)
{
    fDetectorID = other.fDetectorID;
    fTrackID = other.fTrackID;
    fData = other.fData;

    return *this;
}

G4bool DetectorHit::CheckIDMatch(G4int detectorID, G4int trackID)
{
    return (detectorID == fDetectorID) && (trackID == fTrackID);
}

}