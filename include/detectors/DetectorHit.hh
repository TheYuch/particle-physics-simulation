#pragma once

#include <G4VHit.hh>
#include <G4THitsCollection.hh>
#include <G4Allocator.hh>
#include <G4ThreeVector.hh>

#include "hitdata/HitData.hh"

class G4Step;

class DetectorHit : public G4VHit
{
public:
    DetectorHit() = delete;
    DetectorHit(HitData* data) : G4VHit(), fData(data) {}
    ~DetectorHit() { delete fData; }

    // operators
    inline void* operator new(size_t);
    inline void operator delete(void*);

    virtual void Print() override { fData->Print(); }

    // functions
    HitData* GetData() const { return fData; }

protected:
    HitData* fData;
};

typedef G4THitsCollection<DetectorHit> DetectorHitsCollection;

static G4ThreadLocal G4Allocator<DetectorHit>* DetectorHitAllocator = nullptr;

inline void* DetectorHit::operator new(size_t)
{
    if (!DetectorHitAllocator) DetectorHitAllocator = new G4Allocator<DetectorHit>;
    return (void*)DetectorHitAllocator->MallocSingle();
}

inline void DetectorHit::operator delete(void* hit)
{
    DetectorHitAllocator->FreeSingle((DetectorHit*)hit);
}