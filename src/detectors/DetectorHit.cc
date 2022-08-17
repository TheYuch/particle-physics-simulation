#include "detectors/DetectorHit.hh"

namespace Test // TODO: make this into ONE .hh class! check if G4ThreadLocal extern part works though.
{

G4ThreadLocal G4Allocator<DetectorHit>* DetectorHitAllocator = nullptr;

}