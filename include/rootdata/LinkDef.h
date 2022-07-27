// .h so that it does not get GLOB'ed

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class CaloRootData+;
#pragma link C++ class SipmRootData+;

#endif