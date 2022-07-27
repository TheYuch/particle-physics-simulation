#pragma once

#include <G4VHit.hh>
#include <G4THitsCollection.hh>
#include <G4Allocator.hh>
#include <G4ThreeVector.hh>

namespace Test
{

class SipmHit : public G4VHit
{
public:
    SipmHit();
    ~SipmHit();

    // operators
    const SipmHit& operator=(const SipmHit& other);
    inline void* operator new(size_t);
    inline void operator delete(void*);
    
    G4bool CheckIDMatch(G4int sipmID, G4int trackID);

    // functions
    void AddData(G4double time);

    // override
    void Print() override;
    
    // setters
    void SetSipmID(G4int sipmID) { fSipmID = sipmID; }
    void SetTrackID(G4int trackID) { fTrackID = trackID; }

    // getters
    G4int GetSipmID() const { return fSipmID; }
    G4int GetTrackID() const { return fTrackID; }
    G4double GetTime() const { return fTime; }

private:
    G4int fSipmID;
    G4int fTrackID;
    G4double fTime;
};

typedef G4THitsCollection<SipmHit> SipmHitsCollection;

extern G4ThreadLocal G4Allocator<SipmHit>* SipmHitAllocator;

inline void* SipmHit::operator new(size_t)
{
    if (!SipmHitAllocator) SipmHitAllocator = new G4Allocator<SipmHit>;
    return (void*)SipmHitAllocator->MallocSingle();
}

inline void SipmHit::operator delete(void* hit)
{
    SipmHitAllocator->FreeSingle((SipmHit*)hit);
}

}