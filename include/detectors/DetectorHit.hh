#pragma once

#include <G4VHit.hh>
#include <G4THitsCollection.hh>
#include <G4Allocator.hh>
#include <G4ThreeVector.hh>

#include "detectors/HitData.hh"

namespace Test
{

class DetectorHit : public G4VHit
{
public:
    DetectorHit();
    ~DetectorHit();

    // operators
    const DetectorHit& operator=(const DetectorHit& other);
    inline void* operator new(size_t);
    inline void operator delete(void*);
    
    G4bool CheckIDMatch(G4int detectorID, G4int trackID) const;

    // functions
    void UpdateData(HitData* data) { fData->Update(data); }
    void Print() { fData->Print(); }

    // setters
    void SetDetectorID(G4int detectorID) { fDetectorID = detectorID; }
    void SetTrackID(G4int trackID) { fTrackID = trackID; }

    // getters
    G4int GetDetectorID() const { return fDetectorID; }
    G4int GetTrackID() const { return fTrackID; }
    HitData* GetData() const { return fData; }

protected:
    G4int fDetectorID;
    G4int fTrackID;
    HitData* fData;
};

typedef G4THitsCollection<DetectorHit> DetectorHitsCollection;

extern G4ThreadLocal G4Allocator<DetectorHit>* DetectorHitAllocator;

inline void* DetectorHit::operator new(size_t)
{
    if (!DetectorHitAllocator) DetectorHitAllocator = new G4Allocator<DetectorHit>;
    return (void*)DetectorHitAllocator->MallocSingle();
}

inline void DetectorHit::operator delete(void* hit)
{
    DetectorHitAllocator->FreeSingle((DetectorHit*)hit);
}

}